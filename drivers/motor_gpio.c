/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: 电机IO口底层配置
 */

#include "motor_gpio.h"

motor Motors[4];

/**
 *@function 电机IO初始化
 *@param void
 *@return void
 */
void motor_io_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  /* 使能GPIO外设 */

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	/* 关闭JTAG，这样才能将PB3和PB4当做普通IO口使用 */

	/* 配置PWM脚 */
	Motors[0].motor_gpio[0].GPIO_Pin = GPIO_Pin_6;
	Motors[1].motor_gpio[0].GPIO_Pin = GPIO_Pin_7;
	Motors[2].motor_gpio[0].GPIO_Pin = GPIO_Pin_8;
	Motors[3].motor_gpio[0].GPIO_Pin = GPIO_Pin_9;
		
	/* 配置方向脚 */
	Motors[0].motor_gpio[1].GPIO_Pin = GPIO_Pin_3;
	Motors[1].motor_gpio[1].GPIO_Pin = GPIO_Pin_9;
	Motors[2].motor_gpio[1].GPIO_Pin = GPIO_Pin_15;
	Motors[3].motor_gpio[1].GPIO_Pin = GPIO_Pin_12;
	
	Motors[0].motor_gpio[2].GPIO_Pin = GPIO_Pin_4;
	Motors[1].motor_gpio[2].GPIO_Pin = GPIO_Pin_10;
	Motors[2].motor_gpio[2].GPIO_Pin = GPIO_Pin_14;
	Motors[3].motor_gpio[2].GPIO_Pin = GPIO_Pin_11;
	
	/* 配置IO模式 */
	for(int i=0;i<4;++i)
		for(int n=0;n<3;++n)
		{
			if(n == 0)
			{
				Motors[i].motor_gpio[n].GPIO_Mode = GPIO_Mode_AF_PP;
				Motors[i].motor_gpio[n].GPIO_Speed = GPIO_Speed_50MHz;				
			}else
			{
				Motors[i].motor_gpio[n].GPIO_Mode = GPIO_Mode_Out_PP;
				Motors[i].motor_gpio[n].GPIO_Speed = GPIO_Speed_50MHz;	
			}
		}
		
	Motors[0].motor_gpio_type[0] = GPIOB;	
	Motors[0].motor_gpio_type[1] = GPIOB;
	Motors[0].motor_gpio_type[2] = GPIOB;
		
	Motors[1].motor_gpio_type[0] = GPIOB;
	Motors[1].motor_gpio_type[1] = GPIOA;
	Motors[1].motor_gpio_type[2] = GPIOA;
		
	Motors[2].motor_gpio_type[0] = GPIOB;
	Motors[2].motor_gpio_type[1] = GPIOB;
	Motors[2].motor_gpio_type[2] = GPIOB;

	Motors[3].motor_gpio_type[0] = GPIOB;
	Motors[3].motor_gpio_type[1] = GPIOB;
	Motors[3].motor_gpio_type[2] = GPIOB;
		
	/* 初始化IO */	
	for(int i=0;i<4;++i)
		for(int n=0;n<3;++n)
			GPIO_Init(Motors[i].motor_gpio_type[n], &(Motors[i].motor_gpio[n]));
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
