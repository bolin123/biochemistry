#include <stdio.h>
#include <string.h>
#include "mproto.h"
#include "Sys.h"

#define MAX_STEPS_COUNT 10
#define MPROTO_FRAME_BUFFER_LEN 64
#define MPROTO_FRAME_PREAMBLE  0xbc
#define MPROTO_DEFAULT_HOST_ADDR 0xfe

static bool g_frameRecved = false;
static uint8_t g_frameBuffer[MPROTO_FRAME_BUFFER_LEN] = {0};
static uint8_t g_frameDataCount = 0;
static uint8_t g_msgIndex = 0;
static uint32_t	g_broadcastTime = 0;
static bool g_broadcastFlag = false;
static SysEvent_cb g_eventCb = NULL;
static uint8_t g_controlIndex = 0;
static MProtoStep_t g_steps[MAX_STEPS_COUNT];

static uint8_t checkFrame(const uint8_t *dat, uint8_t len)
{
    uint8_t count = 0;
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        count += dat[i];
    }
    return count;
}

void MprotoSend(uint8_t cmd, const uint8_t *dat, uint8_t len, uint8_t sn, bool needAck)
{
    uint8_t buffer[MPROTO_FRAME_BUFFER_LEN];
    MProtoHead_t *proto = (MProtoHead_t *)buffer;

    proto->preamble = MPROTO_FRAME_PREAMBLE;
    proto->addr = MPROTO_DEFAULT_HOST_ADDR;
    proto->dataLen = len;
    proto->sn = sn;
    proto->cmd = cmd;
    if(needAck)
    {
        proto->cmd |= 0x80;
    }
    memcpy(&buffer[sizeof(MProtoHead_t)], dat, len);
    buffer[sizeof(MProtoHead_t) + len] = checkFrame(buffer, sizeof(MProtoHead_t) + len);

    HalUartSend(buffer, sizeof(MProtoHead_t) + len + 1);
}

static void controlOpt(const uint8_t *contents)
{
    uint8_t i = 0, j, n;
    uint8_t *steps = NULL;
    uint8_t stepCount = 0, type, replay;

    g_controlIndex = contents[i++];
    stepCount = contents[i++];
    type = contents[i++];
    replay = contents[i++];
    steps = &contents[i];
    if(stepCount <= MAX_STEPS_COUNT)
    {
        for(n = 0, j = 0; n < stepCount; n++)
        {
            g_steps[n].id = steps[j++];
            g_steps[n].dir = steps[j++];
            g_steps[n].count = steps[j++];
            g_steps[n].count = (g_steps[n].count << 8) + steps[j++];
        }
        g_eventCb(SYS_EVENT_MOTOR_CONTRL, g_steps, stepCount);
    }
    else
    {
        MProtoCtrlResult(1);
    }

}

void MProtoCtrlResult(uint8_t result)
{
    uint8_t data[2];
    data[0] = g_controlIndex;
    data[1] = result;
    MprotoSend(MPROTO_CMD_MOTOR_CTRL, data, 2, g_msgIndex++, 0);
}

static void mprotoRecv(uint8_t byte)
{
    static uint8_t frameDataLen = 0;

    if(g_frameDataCount >= MPROTO_FRAME_BUFFER_LEN)
    {
        g_frameDataCount = 0;
    }
    g_frameBuffer[g_frameDataCount++] = byte;

    switch(g_frameDataCount)
    {
    case 1:
        if(byte != MPROTO_FRAME_PREAMBLE)
        {
            g_frameDataCount = 0;
        }
        break;
    case 2:
        if(byte != SysGetDevAddr() && byte != 0xff)
        {
            g_frameDataCount = 0;
        }
        break;
    case 3:
        if(byte > (MPROTO_FRAME_BUFFER_LEN - sizeof(MProtoHead_t)))
        {
            g_frameDataCount = 0;
        }
        else
        {
            frameDataLen = byte;
        }
        break;
    default:
        if(g_frameDataCount == (sizeof(MProtoHead_t) + frameDataLen + 1))
        {
            if(byte == checkFrame(g_frameBuffer, sizeof(MProtoHead_t) + frameDataLen))
            {
                g_frameRecved = true;
            }
            g_frameDataCount = 0;
        }
        break;
    }
}

static void mprotoPrase(void)
{
    MProtoHead_t *mproto;
    uint8_t cmd;

    if(g_frameRecved)
    {
        mproto = (MProtoHead_t *)g_frameBuffer;

        if(mproto->cmd & 0x80) //need ack
        {
            MprotoSend(MPROTO_CMD_ACK, NULL, 0, mproto->sn, false);
        }
        cmd = mproto->cmd & 0x7f;
        switch(cmd)
        {
        case MPROTO_CMD_BROADCAST:
            g_broadcastTime = SysTime();
            g_broadcastFlag = true;
            break;
        case MPROTO_CMD_SELF_CHECK:
            g_eventCb(SYS_EVENT_SELFCHECK, NULL, 0);
            break;
        case MPROTO_CMD_MOTOR_CTRL:
            controlOpt(mproto->data);
            break;
        case MPROTO_CMD_QUERY:
            break;
        default:
            break;
        }
        g_frameRecved = false;
    }
}

static void broadcastAck(void)
{
    uint8_t addr;
    if(g_broadcastFlag && SysHasPassTime(g_broadcastTime, SysGetDevAddr() * 1000))
    {
        addr = SysGetDevAddr();
        MprotoSend(MPROTO_CMD_BROADCAST, &addr, 1, g_msgIndex++, false);
        g_broadcastFlag = false;
    }
}

void MProtoInit(SysEvent_cb cb)
{
    g_eventCb = cb;
    HalUartInit(mprotoRecv);
}

void MProtoPoll(void)
{
    mprotoPrase();
    broadcastAck();
}

