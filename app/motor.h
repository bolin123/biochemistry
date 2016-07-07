#ifndef MOTOR_H
#define MOTOR_H

#include "ctypes.h"

typedef enum
{
	MOTOR_DIR_FORWARD = 0,
	MOTOR_DIR_BACKWARD,
}MotorDirection_t;

typedef void (*motorCountOver_cb)(uint8_t );
typedef bool (*MotorStopEvent_cb)(void);

void MotorInit(void);
void MotorStart(uint8_t index, MotorDirection_t dir, uint8_t count, motorCountOver_cb cb);
void MotorStop(uint8_t index);
void MotorPoll(void);
void MotorStopEventRegister(uint8_t index, MotorStopEvent_cb cb);

#endif

