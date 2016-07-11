#include "motor.h"
#include "Hal.h"

#define MOTOR_NUM 2 //控制电机总数

static uint8_t g_motorNum = 0;
static MotorSensorTriggered g_sensorTriggered[MOTOR_NUM] = {0};
static MotorEventHandle g_eventCb = NULL;

static void motorStepOverHandle(uint8_t id)
{
    g_eventCb(id, MOTOR_EVENT_STEP_OVER);
}

void MotorInit(MotorEventHandle cb)
{
    HalPulseInfo_t info;

    g_eventCb = cb;
    //MOTOR_INDEX_NUM1
    info.enable = false;
    info.val = 0;
    info.count = 0;
    info.curNum = 0;
    info.enablePin = HAL_MOTOR1_ENABLE_PIN;
    info.ctrlPin = HAL_MOTOR1_CONTRL_PIN;
    info.dirPin = HAL_MOTOR1_DIRECTION_PIN;
    HalPulseInfoInit(0, &info);
    g_motorNum++;

    info.enablePin = HAL_MOTOR2_ENABLE_PIN;
    info.ctrlPin = HAL_MOTOR2_CONTRL_PIN;
    info.dirPin = HAL_MOTOR2_DIRECTION_PIN;
    HalPulseInfoInit(1, &info);
    g_motorNum++;

    HalPulseStart(1, motorStepOverHandle);// 2ms
}

void MotorTriggeredRegister(uint8_t index, MotorSensorTriggered cb)
{
    g_sensorTriggered[index] = cb;
}

void MotorStart(uint8_t index, MotorDirection_t dir, uint8_t count)
{
    HalIRQEnableSet(false);
    HalPulseInfoSet(index, dir, count, true);
    HalIRQEnableSet(true);
}

void MotorStop(uint8_t index)
{
    HalIRQEnableSet(false);
    HalPulseInfoSet(index, 0, 0, false);
    HalIRQEnableSet(true);
}

void MotorPoll(void)
{
    uint8_t i;

    for(i = 0; i < MOTOR_NUM; i++)
    {
        if(g_sensorTriggered[i] != NULL && g_sensorTriggered[i]())
        {
            g_eventCb(i, MOTOR_EVENT_SENSOR_TRIGGERED);
        }
    }
}

