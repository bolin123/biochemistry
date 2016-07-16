#ifndef MPROTO_H
#define MPROTO_H

#include "Sys.h"

//��������������
#define MPROTO_MAX_STEPS_COUNT 10

//Э��ͷ
typedef struct
{
    uint8_t preamble;
    uint8_t addr;
    uint8_t dataLen;
    uint8_t sn;
    uint8_t cmd;
    //uint8_t data[];
} MProtoHead_t;

//�������
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

//�������
typedef enum
{
    MPROTO_RESULT_SUCCESS = 0, //�ɹ�
    MPROTO_RESULT_CMD_VALID,   //ָ����Ч
    MPROTO_RESULT_BUSY,        //�豸æ
    MPROTO_RESULT_SENOR_ERROR, //����������
}MProtoResult_t;

#define MPROTO_CMD_BROADCAST 0x00  //�㲥
#define MPROTO_CMD_SELF_CHECK 0x01 //�Լ�
#define MPROTO_CMD_MOTOR_CTRL 0x02 //�������
#define MPROTO_CMD_QUERY 0x03      //״̬��ѯ
#define MPROTO_CMD_ACK 0x04        //Ӧ��

/*
* ����Э�������ʼ��
* @cb ϵͳ�¼��ص�����
*/
void MProtoInit(SysEvent_cb cb);
void MProtoPoll(void);

/*
* Э�鷢�ͺ���
* @cmd ��Ϣ����
* @dat ��Ϣ����
* @len ��Ϣ���ݳ���
* @sn ��Ϣ���к�
* @needAck �Ƿ���ҪӦ��
*/
void MprotoSend(uint8_t cmd, const uint8_t *dat, uint8_t len, uint8_t sn, bool needAck);
void MProtoCtrlResult(MProtoResult_t result);

#endif
