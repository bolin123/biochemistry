#include "sys.h"
#include "motor.h"
#include "mproto.h"

static void testMotorCb(uint8_t index)
{
	static bool isOver = false;
	MotorStop(index);
	HalGpioPinValueSet(0x13, isOver);
	if(!isOver)
	{
		MotorStart(1, MOTOR_DIR_BACKWARD, 50, testMotorCb);
		isOver = true;
	}
}

static bool testMotor1NeedStop(void)
{
	if(HalGpioPinValueGet(0x00) == 0) //P00
	{
		return true;
	}
	return false;
}

static bool testMotor2NeedStop(void)
{
	if(HalGpioPinValueGet(0x01))  //P01
	{
		return true;
	}
	return false;
}


static void testMotorInit(void)
{
	MotorStopEventRegister(0, testMotor1NeedStop);
	MotorStopEventRegister(1, testMotor2NeedStop);
	MotorStart(0, MOTOR_DIR_BACKWARD, 0, NULL);
	MotorStart(1, MOTOR_DIR_BACKWARD, 0, NULL);
	//MotorStart(1, MOTOR_DIR_BACKWARD, 50, testMotorCb);
}

static void sysEventHandle(uint8_t event, void *args, uint8_t arglen)
{
	switch(event)
	{
		case SYS_EVENT_SELFCHECK:
			break;
		case SYS_EVENT_MOTOR_CONTRL:
			break;
		default:
			break;
	}
}

void SysInit(void)
{
    HalInit();
	MProtoInit(sysEventHandle);
	MotorInit();
	testMotorInit();
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

