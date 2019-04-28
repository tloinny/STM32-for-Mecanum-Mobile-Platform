/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: 编码器IO口底层配置
 */
 
#include "encoder.h"

GPIO_InitTypeDef encoder[4];


/**
 *@function 编码器IO初始化
 *@param void
 *@return void
 */
void encoder_io_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  /* 使能GPIO外设 */
	
	encoder[0].GPIO_Pin = GPIO_Pin_6;	/* PB6 */
	encoder[1].GPIO_Pin = GPIO_Pin_7;	/* PB7 */
	encoder[2].GPIO_Pin = GPIO_Pin_0;	/* PA0 */
	encoder[3].GPIO_Pin = GPIO_Pin_1;	/* PA1 */
	
	for(int i=0;i<4;++i)
	{
		encoder[i].GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 浮空输入 */
		encoder[i].GPIO_Speed = GPIO_Speed_50MHz;
	}
	
	GPIO_Init(GPIOB, encoder);
	GPIO_Init(GPIOB, encoder+1);
	GPIO_Init(GPIOA, encoder+2);
	GPIO_Init(GPIOA, encoder+3);
}
