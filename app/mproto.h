#ifndef MPROTO_H
#define MPROTO_H

#include "Sys.h"

//电机操作最大步骤数
#define MPROTO_MAX_STEPS_COUNT 10

//协议头
typedef struct
{
    uint8_t preamble;
    uint8_t addr;
    uint8_t dataLen;
    uint8_t sn;
    uint8_t cmd;
    //uint8_t data[];
} MProtoHead_t;

//电机步骤
#pragma pack(1)
typedef struct
{
    uint8_t id;
    uint8_t dir:4;
    uint8_t flag:4;
    uint16_t count;
} MProtoStep_t;
#pragma pack()

typedef struct
{
    uint8_t count;
    MProtoStep_t step[MPROTO_MAX_STEPS_COUNT];
}MProtoStepInfo_t;

//操作结果
typedef enum
{
    MPROTO_RESULT_SUCCESS = 0, //成功
    MPROTO_RESULT_CMD_VALID,   //指令无效
    MPROTO_RESULT_BUSY,        //设备忙
    MPROTO_RESULT_SENOR_ERROR, //传感器故障
}MProtoResult_t;

#define MPROTO_CMD_BROADCAST 0x00  //广播
#define MPROTO_CMD_SELF_CHECK 0x01 //自检
#define MPROTO_CMD_MOTOR_CTRL 0x02 //电机控制
#define MPROTO_CMD_QUERY 0x03      //状态查询
#define MPROTO_CMD_ACK 0x04        //应答

/*
* 串口协议解析初始化
* @cb 系统事件回调函数
*/
void MProtoInit(SysEvent_cb cb);
void MProtoPoll(void);

/*
* 协议发送函数
* @cmd 消息类型
* @dat 消息内容
* @len 消息内容长度
* @sn 消息序列号
* @needAck 是否需要应答
*/
void MprotoSend(uint8_t cmd, const uint8_t *dat, uint8_t len, uint8_t sn, bool needAck);
void MProtoCtrlResult(MProtoResult_t result);

#endif
