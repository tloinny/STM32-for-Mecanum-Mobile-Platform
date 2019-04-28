/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Mecanum Mobile Platform firmware config file
 */
 
#ifndef _SYS_CONF_H_
#define _SYS_CONF_H_

/**
 *@description include  
 *在这里定义整个代码include的头文件
 */

#include "stm32f10x.h"
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "motor_gpio.h"
#include "led.h"
#include "encoder.h"

/**
 *@description define 
 *在这里配置用户代码中的define值
 */


/**
 *@description extern 
 *在这里配置用户代码中的extern值
 */
typedef struct motor
{
	GPIO_InitTypeDef motor_gpio[3];
}motor;
 
extern motor Motors[];
extern u8  TIM3CH1_CAPTURE_STA;
extern u16 TIM3CH1_CAPTURE_VAL;


#endif
