#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#define HAL_IDATA data
#define HAL_XDATA xdata

/*�Ƿ�ʹ��Ӳ����ַ*/
#define HAL_USE_HARDWARE_ADDR 0 // 1

#if HAL_USE_HARDWARE_ADDR  //Ӳ����ַ
#define HAL_ADDR2_PIN 0x07
#define HAL_ADDR1_PIN 0x06
#define HAL_ADDR0_PIN 0x05
#else
#define HAL_SOFTWARE_ADDR 0x01 //�����ַ
#endif

/*�����ת����*/
#define HAL_MOTOR_CYCLE_TIME  1 //2ms

/*��Ч�ܽ�*/
#define HAL_GPIO_INVALID 0xff

/*������ƹܽŶ���*/
#define HAL_MOTOR1_CONTRL_PIN  0x27     //���1���ƹܽ� 
#define HAL_MOTOR1_DIRECTION_PIN 0x10   //���1����ܽ� 
#define HAL_MOTOR1_ENABLE_PIN 0x12      //���1ʹ�ܹܽ� 
#define HAL_MOTOR2_CONTRL_PIN  0x26     //���2���ƹܽ� 
#define HAL_MOTOR2_DIRECTION_PIN 0x11	//���2����ܽ� 
#define HAL_MOTOR2_ENABLE_PIN 0x13      //���2ʹ�ܹܽ� 

#define HAL_MOTOR1_TRIGGER1_PIN 0x00 //���1��λ�жϹܽ�1
#define HAL_MOTOR2_TRIGGER1_PIN 0x01 //���2��λ�жϹܽ�1

/*����ܽ�*/
#define HAL_MOTOR_FORWARD_LEVEL 1    //������ת��ƽ
#define HAL_MOTOR_BACKWARD_LEVEL 0   //������ת��ƽ
/*ʹ�ܹܽ�*/
#define HAL_MOTOR_ENABLE_LEVEL 1     //���ʹ�ܵ�ƽ
#define HAL_MOTOR_DISABLE_LEVEL 0    //�����ֹ��ƽ
/*ʱ�ӹܽ�*/
#define HAL_MOTOR_CLK_DISABLE_LEVEL 0  //���ʱ�Ӳ�ʹ�ܵ�ƽ

#endif

