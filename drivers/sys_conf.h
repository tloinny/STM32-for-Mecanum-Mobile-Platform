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
 *�����ﶨ����������include��ͷ�ļ�
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

/**
 *@description define 
 *�����������û������е�defineֵ
 */
#define a 300
#define b 200
#define TIM3_ARR 10000
#define TIM3_PSC 7199
#define TIM4_ARR 10000
#define TIM4_PSC 7199
#define MAX_RPM 70

/**
 *@description extern 
 *�����������û������е�externֵ
 */
typedef struct motor
{
	GPIO_TypeDef* motor_gpio_type;
	GPIO_InitTypeDef motor_gpio[3];
}motor;
 
extern motor Motors[];
extern u8  TIM3CH1_CAPTURE_STA;
extern u16 TIM3CH1_CAPTURE_VAL;

typedef struct wheel_speed
{
	u8 Vw0;
	u8 Vw1;
	u8 Vw2;
	u8 Vw3;
}wheel_speed;
extern wheel_speed W_S;

typedef struct car_speed
{
	u8 Vx;
	u8 Vy;
	u8 W;
}car_speed;
extern car_speed C_S;

#endif
