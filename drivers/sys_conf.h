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
#include "motor_operate.h"
#include "inverse_solution.h"
#include "pid.h"

/**
 *@description define 
 *在这里配置用户代码中的define值
 */
#define a 0.0824
#define b 0.0665
#define TIM3_ARR 0XFFFF
#define TIM3_PSC 71
#define TIM4_ARR 1000
#define TIM4_PSC 7199
#define MAX_RPM 70

/**
 *@description extern 
 *在这里配置用户代码中的extern值
 */
typedef struct motor
{
	GPIO_TypeDef* motor_gpio_type[3];
	GPIO_InitTypeDef motor_gpio[3];
}motor;
 
extern motor Motors[];
extern u8  TIM3CH1_CAPTURE_STA;
extern u16 TIM3CH1_CAPTURE_VAL;
extern u8  TIM3CH2_CAPTURE_STA;
extern u16 TIM3CH2_CAPTURE_VAL;
extern u8  TIM3CH3_CAPTURE_STA;
extern u16 TIM3CH3_CAPTURE_VAL;
extern u8  TIM3CH4_CAPTURE_STA;
extern u16 TIM3CH4_CAPTURE_VAL;

typedef struct wheel_speed
{
	float Vw0;
	float Vw1;
	float Vw2;
	float Vw3;
}wheel_speed;
extern wheel_speed W_S;

typedef struct car_speed
{
	float Vx;
	float Vy;
	float W;
}car_speed;
extern car_speed C_S;

#endif
