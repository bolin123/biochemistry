#include "sys.h"
#include "motor.h"
#include "mproto.h"

static MProtoStep_t *g_curStepInfo = NULL;
static uint8_t g_stepCount = 0;

static bool sysMotor1SensorTriggered(void)
{
    if(HalGpioPinValueGet(0x00) == 0) //P00
    {
        return true;
    }
    return false;
}

static bool sysMotor2SensorTriggered(void)
{
    if(HalGpioPinValueGet(0x01))  //P01
    {
        return true;
    }
    return false;
}


static void sysMotorInit(void)
{
    MotorTriggeredRegister(0, sysMotor1SensorTriggered);
    MotorTriggeredRegister(1, sysMotor2SensorTriggered);
    //MotorStart(0, MOTOR_DIR_BACKWARD, 0, NULL);
    //MotorStart(1, MOTOR_DIR_BACKWARD, 0, NULL);
    //MotorStart(1, MOTOR_DIR_BACKWARD, 50, testMotorCb);
}

static void sysMotorEventHandle(uint8_t id, MotorEvent_t event)
{
    bool doNextStep = false;

    if(MOTOR_EVENT_STEP_OVER == event)
    {
        doNextStep = true;
    }
    else if(MOTOR_EVENT_SENSOR_TRIGGERED == event)
    {
        if(g_curStepInfo->count == 0)
        {
            doNextStep = true;
        }
    }

    if(doNextStep)
    {
        MotorStop(id);
        if(g_stepCount == 0)
        {
            MProtoCtrlResult(0);
            g_curStepInfo = NULL;
        }
        else
        {
            g_curStepInfo++;
            g_stepCount--;
            MotorStart(g_curStepInfo->id, g_curStepInfo->dir, g_curStepInfo->count);
        }
    }
}

static void sysEventHandle(uint8_t event, void *args)
{
    MProtoStepInfo_t *stepInfo = NULL;
    switch(event)
    {
    case SYS_EVENT_SELFCHECK:
        break;
    case SYS_EVENT_MOTOR_CONTRL:
        stepInfo = (MProtoStepInfo_t *)args;
        g_curStepInfo = stepInfo->step;
        g_stepCount = stepInfo->count - 1;
        MotorStart(g_curStepInfo->id, g_curStepInfo->dir, g_curStepInfo->count);
        break;
    default:
        break;
    }
}

void SysInit(void)
{
    HalInit();
    MProtoInit(sysEventHandle);
    MotorInit(sysMotorEventHandle);
    sysMotorInit();
}

void SysPoll(void)
{
    MotorPoll();
    MProtoPoll();
}

uint8_t SysGetDevAddr(void)
{
    return HalGetDevAddr();
}

