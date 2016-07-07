#include "motor.h"
#include "Hal.h"

#define MOTOR_NUM 2 //控制电机总数

#define MOTOR1_CONTRL_PIN  0x27     //电机1控制管脚 p27
#define MOTOR1_DIRECTION_PIN 0x25   //电机1方向管脚 p17
#define MOTOR2_CONTRL_PIN  0x26     //电机2控制管脚 p26
#define MOTOR2_DIRECTION_PIN 0x24	//电机2方向管脚 p16

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
    info.cb = NULL;
    info.curNum = 0;
    info.enablePin = HAL_GPIO_INVALID;
    info.ctrlPin = MOTOR1_CONTRL_PIN;
    info.dirPin = MOTOR1_DIRECTION_PIN;
    HalPulseInfoInit(0, &info);
    g_motorNum++;

    info.ctrlPin = MOTOR2_CONTRL_PIN;
    info.dirPin = MOTOR2_DIRECTION_PIN;
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
    HalPulseInfo_t *info;
    uint8_t i;

    for(i = 0; i < MOTOR_NUM; i++)
    {
        if(g_sensorTriggered[i] != NULL && g_sensorTriggered[i]())
        {
            g_eventCb(i, MOTOR_EVENT_SENSOR_TRIGGERED);
        }
    }
}

