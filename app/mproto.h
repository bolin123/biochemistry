#ifndef MPROTO_H
#define MPROTO_H

#include "ctypes.h"

typedef struct
{
	uint8_t preamble;
	uint8_t addr;
	uint8_t dataLen;
	uint8_t sn;
	uint8_t cmd;
	uint8_t data[];
}MProtoHead_t;

typedef struct
{
	uint8_t id;
	uint8_t dir;
	uint16_t count;
}MProtoStep_t;
								 
#define MPROTO_CMD_BROADCAST 0x00
#define MPROTO_CMD_SELF_CHECK 0x01
#define MPROTO_CMD_MOTOR_CTRL 0x02
#define MPROTO_CMD_QUERY 0x03
#define MPROTO_CMD_ACK 0x04

void MProtoInit(SysEvent_cb cb);
void MProtoPoll(void);
void MprotoSend(uint8_t cmd, const uint8_t *dat, uint8_t len, uint8_t sn, bool needAck);
#endif
