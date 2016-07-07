#ifndef HAL_H
#define HAL_H
#include "ctypes.h"
#include "motor.h"
#include "HalConfig.h"

#define HAL_GPIO_INVALID 0xff

typedef struct
{
    bool enable;        //ʹ��λ
    uint8_t ctrlPin;    //���ƹܽ�
    uint8_t dirPin;     //����ܽ�
    uint8_t enablePin;  //ʹ�ܹܽ�
    MotorDirection_t dir : 8; //����
    uint8_t count;    //�����ܲ��� (0Ϊ����)
    uint8_t curNum;   //��ǰ����
    uint8_t val;      //���ƹܽŸߵ͵�ƽ
    //	MotorStepOver_cb cb;  //�趨������ɻص�����
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
