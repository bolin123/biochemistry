#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#define HAL_IDATA data
#define HAL_XDATA xdata

/*是否使用硬件地址*/
#define HAL_USE_HARDWARE_ADDR 0 // 1

#if HAL_USE_HARDWARE_ADDR  //硬件地址
#define HAL_ADDR2_PIN 0x07
#define HAL_ADDR1_PIN 0x06
#define HAL_ADDR0_PIN 0x05
#else
#define HAL_SOFTWARE_ADDR 0x01 //软件地址
#endif

/*电机旋转周期*/
#define HAL_MOTOR_CYCLE_TIME  1 //2ms

/*无效管脚*/
#define HAL_GPIO_INVALID 0xff

/*电机控制管脚定义*/
#define HAL_MOTOR1_CONTRL_PIN  0x27     //电机1控制管脚 
#define HAL_MOTOR1_DIRECTION_PIN 0x10   //电机1方向管脚 
#define HAL_MOTOR1_ENABLE_PIN 0x12      //电机1使能管脚 
#define HAL_MOTOR2_CONTRL_PIN  0x26     //电机2控制管脚 
#define HAL_MOTOR2_DIRECTION_PIN 0x11	//电机2方向管脚 
#define HAL_MOTOR2_ENABLE_PIN 0x13      //电机2使能管脚 

#define HAL_MOTOR1_TRIGGER1_PIN 0x00 //电机1定位中断管脚1
#define HAL_MOTOR2_TRIGGER1_PIN 0x01 //电机2定位中断管脚1

/*方向管脚*/
#define HAL_MOTOR_FORWARD_LEVEL 1    //正向旋转电平
#define HAL_MOTOR_BACKWARD_LEVEL 0   //反向旋转电平
/*使能管脚*/
#define HAL_MOTOR_ENABLE_LEVEL 1     //电机使能电平
#define HAL_MOTOR_DISABLE_LEVEL 0    //电机禁止电平
/*时钟管脚*/
#define HAL_MOTOR_CLK_DISABLE_LEVEL 0  //电机时钟不使能电平

#endif

