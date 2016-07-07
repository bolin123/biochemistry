#ifndef HAL_H
#define HAL_H
#include "ctypes.h"
#include "motor.h"
#include "HalConfig.h"

#define HAL_GPIO_INVALID 0xff

typedef struct
{
    bool enable;        //使能位
    uint8_t ctrlPin;    //控制管脚
    uint8_t dirPin;     //方向管脚
    uint8_t enablePin;  //使能管脚
    MotorDirection_t dir : 8; //方向
    uint8_t count;    //设置总步数 (0为无限)
    uint8_t curNum;   //当前步数
    uint8_t val;      //控制管脚高低电平
    //	MotorStepOver_cb cb;  //设定步数完成回调函数
} HalPulseInfo_t;

typedef void (* HalUartRecv_cb)(uint8_t);
void HalUartInit(HalUartRecv_cb cb);
void HalUartSend(const uint8_t *dat, uint8_t len);

void HalInit(void);
uint32_t HalSysTime(void);
uint8_t HalGpioPinValueGet(uint8_t io);
void HalGpioPinValueSet(uint8_t io, uint8_t val);

void HalIRQEnableSet(bool enable);

int8_t HalPulseInfoInit(uint8_t index, HalPulseInfo_t *info);
int8_t HalPulseInfoSet(uint8_t index, MotorDirection_t dir, uint8_t count, bool enable);
HalPulseInfo_t *HalPulseInfoGet(uint8_t index);
void HalPulseStart(uint32_t period, MotorStepOver_cb cb);
void HalPulseStop(void);

uint8_t HalGetDevAddr(void);

#endif
