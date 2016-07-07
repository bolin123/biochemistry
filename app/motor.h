#ifndef MOTOR_H
#define MOTOR_H

#include "ctypes.h"

typedef enum
{
    MOTOR_DIR_FORWARD = 0,
    MOTOR_DIR_BACKWARD,
} MotorDirection_t;

typedef enum
{
    MOTOR_EVENT_STEP_OVER = 0,
    MOTOR_EVENT_SENSOR_TRIGGERED,
} MotorEvent_t;

typedef void (*MotorStepOver_cb)(uint8_t );
typedef bool (*MotorSensorTriggered)(void);
typedef void (*MotorEventHandle)(uint8_t, MotorEvent_t);

void MotorInit(MotorEventHandle cb);
void MotorStart(uint8_t index, MotorDirection_t dir, uint8_t count, MotorStepOver_cb cb);
void MotorStop(uint8_t index);
void MotorPoll(void);
void MotorTriggeredRegister(uint8_t index, MotorSensorTriggered cb);

#endif

