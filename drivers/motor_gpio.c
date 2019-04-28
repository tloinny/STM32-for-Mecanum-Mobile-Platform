/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: ���IO�ڵײ�����
 */

#include "motor_gpio.h"

motor Motors[4];

/**
 *@function ���IO��ʼ��
 *@param void
 *@return void
 */
void motor_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  /* ʹ��GPIO���� */

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	/* �ر�JTAG���������ܽ�PB3��PB4������ͨIO��ʹ�� */
	
	/* ����PWM�� */
	Motors[0].motor_gpio[0].GPIO_Pin = GPIO_Pin_6;
	Motors[1].motor_gpio[0].GPIO_Pin = GPIO_Pin_7;
	Motors[2].motor_gpio[0].GPIO_Pin = GPIO_Pin_8;
	Motors[3].motor_gpio[0].GPIO_Pin = GPIO_Pin_9;
	
	/* ���÷���� */
	Motors[0].motor_gpio[1].GPIO_Pin = GPIO_Pin_3;
	Motors[1].motor_gpio[1].GPIO_Pin = GPIO_Pin_9;
	Motors[2].motor_gpio[1].GPIO_Pin = GPIO_Pin_15;
	Motors[3].motor_gpio[1].GPIO_Pin = GPIO_Pin_12;
	
	Motors[0].motor_gpio[2].GPIO_Pin = GPIO_Pin_4;
	Motors[1].motor_gpio[2].GPIO_Pin = GPIO_Pin_10;
	Motors[2].motor_gpio[2].GPIO_Pin = GPIO_Pin_14;
	Motors[3].motor_gpio[2].GPIO_Pin = GPIO_Pin_11;
	
	for(int i=0;i<4;++i)
		for(int n=1;n<3;++n)
		{
			Motors[i].motor_gpio[n].GPIO_Mode = GPIO_Mode_AF_PP;
			Motors[i].motor_gpio[n].GPIO_Speed = GPIO_Speed_50MHz;
		}
	
	/* ��ʼ��GPIO */
	GPIO_Init(GPIOB, &(Motors[0].motor_gpio[0]));
	GPIO_Init(GPIOB, &(Motors[0].motor_gpio[1]));
	GPIO_Init(GPIOB, &(Motors[0].motor_gpio[2]));
	
	GPIO_Init(GPIOB, &(Motors[1].motor_gpio[0]));
	GPIO_Init(GPIOA, &(Motors[1].motor_gpio[1]));
	GPIO_Init(GPIOA, &(Motors[1].motor_gpio[2]));
	
	GPIO_Init(GPIOB, &(Motors[2].motor_gpio[0]));
	GPIO_Init(GPIOB, &(Motors[2].motor_gpio[1]));
	GPIO_Init(GPIOB, &(Motors[2].motor_gpio[2]));

	GPIO_Init(GPIOB, &(Motors[3].motor_gpio[0]));
	GPIO_Init(GPIOB, &(Motors[3].motor_gpio[1]));
	GPIO_Init(GPIOB, &(Motors[3].motor_gpio[2]));
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}