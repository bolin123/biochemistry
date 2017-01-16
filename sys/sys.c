#include "sys.h"
#include "motor.h"
#include "mproto.h"

static HAL_XDATA MProtoStep_t *g_curStepInfo = NULL;
static HAL_XDATA uint8_t g_stepCount = 0;
static HAL_XDATA bool g_sysStatus = SYS_STATUS_IDLE;

static bool sysMotor1SensorTriggered(void)
{
    if(HalGpioPinValueGet(HAL_MOTOR1_TRIGGER1_PIN)) //P00
    {
        return true;
    }
    return false;
}

static bool sysMotor2SensorTriggered(void)
{
    if(!HalGpioPinValueGet(HAL_MOTOR2_TRIGGER1_PIN))  //P01
    {
        return true;
    }
    return false;
}


static void sysMotorInit(void)
{
    MotorTriggeredRegister(0, sysMotor1SensorTriggered);
    MotorTriggeredRegister(1, sysMotor2SensorTriggered);
}

static void sysMotorEventHandle(uint8_t id, MotorEvent_t event)
{
    bool doNextStep = false;

    if(MOTOR_EVENT_STEP_OVER == event)
    {
        doNextStep = true;
        if(g_curStepInfo->flag == 1) 
        {
            MProtoCtrlResult(MPROTO_RESULT_SENOR_ERROR); //´«¸ÐÆ÷Î´´¥·¢
        }
    }
    else if(MOTOR_EVENT_SENSOR_TRIGGERED == event)
    {
        if(g_curStepInfo != NULL 
            && g_curStepInfo->id == id
            && g_curStepInfo->flag == 1)
        {
            doNextStep = true;
        }
    }

    if(doNextStep)
    {
        MotorStop(id);
        if(g_stepCount == 0)
        {
            g_sysStatus = SYS_STATUS_IDLE;
            MProtoCtrlResult(MPROTO_RESULT_SUCCESS);
            g_curStepInfo = NULL;
        }
        else
        {
            g_curStepInfo++;
            g_stepCount--;
            MotorStart(g_curStepInfo->id, g_curStepInfo->dir, g_curStepInfo->cycle);
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
        g_sysStatus = SYS_STATUS_BUSY;
        MotorStart(g_curStepInfo->id, g_curStepInfo->dir, g_curStepInfo->cycle);
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

SysStatus_t SysGetStatus(void)
{
    return g_sysStatus;
}

uint8_t SysGetDevAddr(void)
{
    return HalGetDevAddr();
}

