#include "motor.h"
#include "Hal.h"

#define MOTOR_NUM 2 //控制电机总数

#define MOTOR1_CONTRL_PIN  0x27     //电机1控制管脚 p27
#define MOTOR1_DIRECTION_PIN 0x25   //电机1方向管脚 p17
#define MOTOR2_CONTRL_PIN  0x26     //电机2控制管脚 p26
#define MOTOR2_DIRECTION_PIN 0x24	//电机2方向管脚 p16

static uint8_t g_motorNum = 0;
static MotorStopEvent_cb g_stopEvent[MOTOR_NUM] = {0};

void MotorInit(void)
{
	HalPulseInfo_t info;
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
	
	HalPulseStart(1);// 2ms
}

void MotorStopEventRegister(uint8_t index, MotorStopEvent_cb cb)
{
	g_stopEvent[index] = cb;
}

void MotorStart(uint8_t index, MotorDirection_t dir, uint8_t count, motorCountOver_cb cb)
{
	HalIRQEnableSet(false);
	HalPulseInfoSet(index, dir, count, true, cb);
	HalIRQEnableSet(true);
}

void MotorStop(uint8_t index)
{
	HalIRQEnableSet(false);
	HalPulseInfoSet(index, 0, 0, false, NULL);
	HalIRQEnableSet(true);
}

void MotorPoll(void)
{
	HalPulseInfo_t *info;
	uint8_t i;
	
	for(i = 0; i < MOTOR_NUM; i++)
	{
		if(g_stopEvent[i] != NULL && g_stopEvent[i]())
		{
			info = HalPulseInfoGet(i);
		
			if(info->enable)
			{
				MotorStop(i);
			}
			
		}
	}
	#if 0
	static bool isStop1 = false;
	static bool isStop2 = false;
	
	if(HalGpioPinValueGet(0x00) == 0)
	{
        if(!isStop1)
        {
            isStop1 = true;
            MotorStop(0);
        }
	}

	if(HalGpioPinValueGet(0x01))
	{
		if(!isStop2)
		{
			isStop2 = true;
			MotorStop(1);
		}
	}
	#endif
}

