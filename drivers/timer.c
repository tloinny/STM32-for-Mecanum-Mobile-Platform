/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: 定时器底层配置
 */ 
#include "timer.h"

TIM_ICInitTypeDef encoders_channel[4];

u8  TIM3CH1_CAPTURE_STA=0;
u16 TIM3CH1_CAPTURE_VAL;
 
/**
 *@function TIM3 初始化 
 *@param 
 *			arr：自动重装值
 *			psc：时钟预分频数
 */
void TIM3_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* 使能定时器3时钟 */
 
   /* 初始化TIM3 */
	TIM_TimeBaseStructure.TIM_Period = arr; /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM向上计数模式 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */
	
	/* 初始化捕获通道 */
	TIM_ICInitTypeDef TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; /* 输入映射到TI1 */
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	/* 下降沿捕获 */
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;	/* 不滤波 */
	
	encoders_channel[0] = TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;
	encoders_channel[1] = TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;
	encoders_channel[2] = TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;
	encoders_channel[3] = TIM3_ICInitStructure;
	
  TIM_ICInit(TIM3, encoders_channel);
	
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3中断 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* 抢占优先级2 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* 从优先级0 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* 使能中断 */
  NVIC_Init(&NVIC_InitStructure);
		
	TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
}

/**
 *@function TIM3 中断服务函数
 *@param void
 */
void TIM3_IRQHandler(void)
{
	if((TIM3CH1_CAPTURE_STA&0X80)==0)	/* 如果尚未完成一次全程捕捉 */
	{      
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	/* 如果发生了更新中断 */
		{        
			if(TIM3CH1_CAPTURE_STA&0X40)	/* 如果只捕捉到了起点，尚未捕捉到终点 */
				{
					if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)	/* 如果超过了能记录的最大范围 */
						{
							TIM3CH1_CAPTURE_STA|=0X80;	/* 强行视为已完成捕获 */
							TIM3CH1_CAPTURE_VAL=0XFFFF;	/* VAL设置为最大值 */
						}else TIM3CH1_CAPTURE_STA++;	/* 如果还没达到最大值，则继续记录 */
				}     
		}
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)	/* 如果发生了捕捉中断 */
		{    
			if(TIM3CH1_CAPTURE_STA&0X40)	/* 如果已经捕捉过起点，则表示此次捕捉到了终点 */
			{                  
				TIM3CH1_CAPTURE_STA|=0X80;	/* 标志为完成一次全程捕捉 */
				TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);	/* 获取当前CNT值 */
			}else	/* 如果还没捕捉过起点，则表示此次捕捉到了起点 */
			{
				TIM3CH1_CAPTURE_STA=0;	/* 清零STA，准备新一轮的记录 */
				TIM3CH1_CAPTURE_VAL=0;	/* 清零VAL */
				TIM_SetCounter(TIM3,0);	/* 清零CNT */
				TIM3CH1_CAPTURE_STA|=0X40;	/* 标志为已经捕捉过起点 */
			}            
		}                                                
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update);
}

/**
 *@function TIM4 PWM 初始化 
 *@param 
 *			arr：自动重装值
 *			psc：时钟预分频数
 */
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
   /* 初始化TIM4 */
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* 初始化TIM4 PWM模式 */	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* 选择定时器模式:TIM脉冲宽度调制模式2 */
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* 输出极性:TIM输出比较极性高 */
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC1 */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC2 */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC3 */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  /* 初始化外设TIM4 OC4 */

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR1上的预装载寄存器 */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR2上的预装载寄存器 */
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR3上的预装载寄存器 */
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* 使能TIM4在CCR4上的预装载寄存器 */
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
}
