#ifndef HAL_H
#define HAL_H
#include "ctypes.h"
#include "motor.h"
#include "HalConfig.h"

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

//系统时钟
uint32_t HalSysTime(void);

//GPIO操作
uint8_t HalGpioPinValueGet(uint8_t io);
void HalGpioPinValueSet(uint8_t io, uint8_t val);

//全局中断 true=使能，false=禁用
void HalIRQEnableSet(bool enable);

//电机控制脉冲相关操作
int8_t HalPulseInfoInit(uint8_t index, HalPulseInfo_t *info);
int8_t HalPulseInfoSet(uint8_t index, MotorDirection_t dir, uint8_t count, bool enable);
HalPulseInfo_t *HalPulseInfoGet(uint8_t index);

/*
* 脉冲开始 
* @period,脉冲周期(ms) 
* @cb，电机旋转达到计数回调函数
*/
void HalPulseStart(uint32_t period, MotorStepOver_cb cb);
void HalPulseStop(void);

//获取硬件设备地址
uint8_t HalGetDevAddr(void);

#endif
