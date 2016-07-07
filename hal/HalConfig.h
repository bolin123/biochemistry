#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#define HAL_USE_HARDWARE_ADDR 0 // 1

#if HAL_USE_HARDWARE_ADDR  //硬件地址
	#define HAL_ADDR2_PIN 0x07
	#define HAL_ADDR1_PIN 0x06
	#define HAL_ADDR0_PIN 0x05
#else
	#define HAL_SOFTWARE_ADDR 0x01 //软件地址
#endif

/*方向管脚*/
#define HAL_MOTOR_FORWARD_LEVEL 0   //正向旋转电平
#define HAL_MOTOR_BACKWARD_LEVEL 1  //反向旋转电平
/*使能管脚*/
#define HAL_MOTOR_ENABLE_LEVEL 0    //电机使能电平
#define HAL_MOTOR_DISABLE_LEVEL 1   //电机禁止电平
/*时钟管脚*/
#define HAL_MOTOR_CLK_DISABLE_LEVEL 1  //电机时钟不使能电平

#endif

