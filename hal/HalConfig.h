#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#define HAL_USE_HARDWARE_ADDR 0 // 1

#if HAL_USE_HARDWARE_ADDR  //Ӳ����ַ
	#define HAL_ADDR2_PIN 0x07
	#define HAL_ADDR1_PIN 0x06
	#define HAL_ADDR0_PIN 0x05
#else
	#define HAL_SOFTWARE_ADDR 0x01 //�����ַ
#endif

/*����ܽ�*/
#define HAL_MOTOR_FORWARD_LEVEL 0   //������ת��ƽ
#define HAL_MOTOR_BACKWARD_LEVEL 1  //������ת��ƽ
/*ʹ�ܹܽ�*/
#define HAL_MOTOR_ENABLE_LEVEL 0    //���ʹ�ܵ�ƽ
#define HAL_MOTOR_DISABLE_LEVEL 1   //�����ֹ��ƽ
/*ʱ�ӹܽ�*/
#define HAL_MOTOR_CLK_DISABLE_LEVEL 1  //���ʱ�Ӳ�ʹ�ܵ�ƽ

#endif

