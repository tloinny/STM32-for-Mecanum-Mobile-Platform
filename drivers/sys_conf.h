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

/**
 *@description define 
 *�����������û������е�defineֵ
 */


/**
 *@description extern 
 *�����������û������е�externֵ
 */
typedef struct motor
{
	GPIO_InitTypeDef motor_gpio[3];
}motor;
 
extern motor Motors[];
extern u8  TIM3CH1_CAPTURE_STA;
extern u16 TIM3CH1_CAPTURE_VAL;


#endif
