#ifndef SYS_H
#define SYS_H
#include "Hal.h"
#include "ctypes.h"

#define SysTime() HalSysTime()
#define SysHasPassTime(oldTime, pass) (SysTime() - oldTime > pass)

#define SYS_EVENT_SELFCHECK 0x00
#define SYS_EVENT_MOTOR_CONTRL 0x01

typedef void (*SysEvent_cb)(uint8_t event, void *args);

void SysInit(void);
void SysPoll(void);
uint8_t SysGetDevAddr(void);

#endif

