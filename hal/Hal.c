#include "stc89c5xrc.h"
#include "Hal.h"

#define HAL_PULSE_INFO_NUM 2

HalPulseInfo_t g_pulseConfig[HAL_PULSE_INFO_NUM];
static uint8_t g_pulseCount[2];
static uint32_t g_timerCount = 0;

void HalGpioPinValueSet(uint8_t io, uint8_t val)
{
    uint8_t port = (io & 0xf0) >> 4;
    uint8_t pin = io & 0x0f;
    uint8_t temp;
    
    switch(port)
    {
        case 0:
            temp = P0;
            if(val)
            {
                temp |= 0x1 << pin;
            }
            else
            {
                temp &= ~(0x1 << pin);
            }
            P0 = temp;
            break;
        case 1:
            temp = P1;
            if(val)
            {
                temp |= 0x1 << pin;
            }
            else
            {
                temp &= ~(0x1 << pin);
            }
            P1 = temp;
            break;
        case 2:
            temp = P2;
            if(val)
            {
                temp |= 0x1 << pin;
            }
            else
            {
                temp &= ~(0x1 << pin);
            }
            P2 = temp;
            break;
        case 3:
            temp = P3;
            if(val)
            {
                temp |= 0x1 << pin;
            }
            else
            {
                temp &= ~(0x1 << pin);
            }
            P3 = temp;
            break;
        case 4:
            temp = P4;
            if(val)
            {
                temp |= 0x1 << pin;
            }
            else
            {
                temp &= ~(0x1 << pin);
            }
            P4 = temp;
            break;
        default:
            break;
    }
}

uint8_t HalGpioPinValueGet(uint8_t io)
{
    uint8_t val;
    uint8_t port = (io & 0xf0) >> 4;
    uint8_t pin = io & 0x0f;
    
    switch(port)
    {
        case 0:
            val = P0 & (0x1 << pin);
            break;
        case 1:
            val = P1 & (0x1 << pin);
            break;
        case 2:
            val = P2 & (0x1 << pin);
            break;
        case 3:
            val = P3 & (0x1 << pin);
            break;
        case 4:
            val = P4 & (0x1 << pin);
            break;
        default:
            break;
    }
    return val;
}

static void timerModeConfig(uint8_t timer0, uint8_t timer1)
{	
	uint8_t config = ((timer1 << 4) & 0xf0) + (timer0 & 0x0f);
	TMOD = config;
}

static void sysTimerInit(void)
{
	TH0  = 0xfc;
	TL0  = 0x66;	//1ms
	TR0 = 1;
	ET0 = 1;
}

void Time0_Int() interrupt 1
{
	TH0 = 0xfc;
	TL0 = 0x66;
	g_timerCount++;
}

void HalIRQEnableSet(bool enable)
{	
    EA = enable;
}

uint32_t HalSysTime(void)
{
    return g_timerCount;
}

int8_t HalPulseInfoInit(uint8_t index, HalPulseInfo_t *info)
{	
	if(index >= HAL_PULSE_INFO_NUM)
	{
		return -1;
	}
	g_pulseConfig[index] = *info;
	return 0;
}

HalPulseInfo_t * HalPulseInfoGet(uint8_t index)
{
	return &g_pulseConfig[index];
}

int8_t HalPulseInfoSet(uint8_t index, MotorDirection_t dir, 
	uint8_t count, bool enable, motorCountOver_cb cb)
{
	if(index >= HAL_PULSE_INFO_NUM)
	{
		return -1;
	}
	g_pulseConfig[index].enable = enable;
	g_pulseConfig[index].val = HAL_MOTOR_CLK_DISABLE_LEVEL; //disable
	g_pulseConfig[index].count = count;
	g_pulseConfig[index].curNum = 0;
	g_pulseConfig[index].dir = dir;
	g_pulseConfig[index].cb = cb;
	if(enable)
	{		
		if(dir == MOTOR_DIR_FORWARD)
		{
			HalGpioPinValueSet(g_pulseConfig[index].dirPin, HAL_MOTOR_FORWARD_LEVEL); //forward
		}
		else
		{
			HalGpioPinValueSet(g_pulseConfig[index].dirPin, HAL_MOTOR_BACKWARD_LEVEL); //backward
		}
		if(g_pulseConfig[index].enablePin != HAL_GPIO_INVALID)
		{
			HalGpioPinValueSet(g_pulseConfig[index].enablePin, HAL_MOTOR_ENABLE_LEVEL);
		}
	}
	else
	{
		if(g_pulseConfig[index].enablePin != HAL_GPIO_INVALID)
		{
			HalGpioPinValueSet(g_pulseConfig[index].enablePin, HAL_MOTOR_DISABLE_LEVEL);
		}
	}
    return 0;
}

/*
	period: x ms
*/

void HalPulseStart(uint32_t period)
{
	float count = 11059200/12/(period*1000);
	uint32_t timeCount = (uint32_t)count;

	g_pulseCount[1] = (0xffff - timeCount)/0xff;
	g_pulseCount[0] = (0xffff - timeCount)%0xff;
	TH1  = g_pulseCount[1];
	TL1  = g_pulseCount[0];
	TR1 = 1; //enable timer 1
	ET1 = 1; //irq enable
}

void HalPulseStop(void)
{
	ET1 = 0;
	TR1 = 0;
}

#define HAL_MOTOR_VALID_LEVEL 0x0

void Time1_Int() interrupt 3
{
	uint8_t i;
	for(i = 0; i < HAL_PULSE_INFO_NUM; i++)
	{
		if(g_pulseConfig[i].enable)
		{
			if(g_pulseConfig[i].count != 0 && g_pulseConfig[i].curNum >= g_pulseConfig[i].count)
			{
				if(g_pulseConfig[i].cb != NULL)
				{
					g_pulseConfig[i].cb(i);
				}
			}
			else
			{
				HalGpioPinValueSet(g_pulseConfig[i].ctrlPin, g_pulseConfig[i].val);
				if(HAL_MOTOR_VALID_LEVEL == g_pulseConfig[i].val)
				{
					g_pulseConfig[i].curNum++;
				}
				g_pulseConfig[i].val = !g_pulseConfig[i].val;
			}
		}
	}
	TH1  = g_pulseCount[1];
	TL1  = g_pulseCount[0];
}

uint8_t HalGetDevAddr(void)
{
#if HAL_USE_HARDWARE_ADDR
	uint8_t addr = 0;
	addr |= HalGpioPinValueGet(HAL_ADDR2_PIN) << 2;
	addr |= HalGpioPinValueGet(HAL_ADDR1_PIN) << 1;
	addr |= HalGpioPinValueGet(HAL_ADDR0_PIN);
	return addr;
#else
	return HAL_SOFTWARE_ADDR;
#endif
}

static HalUartRecv_cb g_uartRecvCb = NULL;

void HalUartInit(HalUartRecv_cb cb)
{
	g_uartRecvCb = cb;
    HalIRQEnableSet(false);
    SCON = 0x50;
    T2CON = 0x34;
    RCAP2H = 0xff;
    RCAP2L = 0xdb;
    TH2 = RCAP2H;
    TL2 = RCAP2L;
    ES = 1;
    HalIRQEnableSet(true);
}

void HalUartSend(const uint8_t *dat, uint8_t len)
{
	uint8_t i;

	for(i = 0; i < len; i++)
	{
		TI = 0;
		SBUF = dat[i];
		while(TI == 0);
		TI = 0;
	}
}

void uartIrq() interrupt 4
{
	if(RI)
    {
		if(g_uartRecvCb != NULL)
		{
			g_uartRecvCb(SBUF);
		}
        RI = 0;
    }
}

void HalInit(void)
{
	timerModeConfig(1, 1);
    sysTimerInit();
	HalIRQEnableSet(true);
}

