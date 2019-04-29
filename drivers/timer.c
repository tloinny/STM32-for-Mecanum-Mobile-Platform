/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: ��ʱ���ײ�����
 */ 
#include "timer.h"

TIM_ICInitTypeDef encoders_channel[4];

u8  TIM3CH1_CAPTURE_STA=0;
u16 TIM3CH1_CAPTURE_VAL;
 
/**
 *@function TIM3 ��ʼ�� 
 *@param 
 *			arr���Զ���װֵ
 *			psc��ʱ��Ԥ��Ƶ��
 */
void TIM3_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	/* ʹ�ܶ�ʱ��3ʱ�� */
 
   /* ��ʼ��TIM3 */
	TIM_TimeBaseStructure.TIM_Period = arr; /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	
	/* ��ʼ������ͨ�� */
	TIM_ICInitTypeDef TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; /* ����ӳ�䵽TI1 */
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	/* �½��ز��� */
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;	/* ���˲� */
	
	encoders_channel[0] = TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;
	encoders_channel[1] = TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;
	encoders_channel[2] = TIM3_ICInitStructure;
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;
	encoders_channel[3] = TIM3_ICInitStructure;
	
  TIM_ICInit(TIM3, encoders_channel);
	
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3�ж� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* ��ռ���ȼ�2 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	/* �����ȼ�0 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	/* ʹ���ж� */
  NVIC_Init(&NVIC_InitStructure);
		
	TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  /* ʹ��TIM3 */
}

/**
 *@function TIM3 �жϷ�����
 *@param void
 */
void TIM3_IRQHandler(void)
{
	if((TIM3CH1_CAPTURE_STA&0X80)==0)	/* �����δ���һ��ȫ�̲�׽ */
	{      
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	/* ��������˸����ж� */
		{        
			if(TIM3CH1_CAPTURE_STA&0X40)	/* ���ֻ��׽������㣬��δ��׽���յ� */
				{
					if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)	/* ����������ܼ�¼�����Χ */
						{
							TIM3CH1_CAPTURE_STA|=0X80;	/* ǿ����Ϊ����ɲ��� */
							TIM3CH1_CAPTURE_VAL=0XFFFF;	/* VAL����Ϊ���ֵ */
						}else TIM3CH1_CAPTURE_STA++;	/* �����û�ﵽ���ֵ���������¼ */
				}     
		}
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)	/* ��������˲�׽�ж� */
		{    
			if(TIM3CH1_CAPTURE_STA&0X40)	/* ����Ѿ���׽����㣬���ʾ�˴β�׽�����յ� */
			{                  
				TIM3CH1_CAPTURE_STA|=0X80;	/* ��־Ϊ���һ��ȫ�̲�׽ */
				TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);	/* ��ȡ��ǰCNTֵ */
			}else	/* �����û��׽����㣬���ʾ�˴β�׽������� */
			{
				TIM3CH1_CAPTURE_STA=0;	/* ����STA��׼����һ�ֵļ�¼ */
				TIM3CH1_CAPTURE_VAL=0;	/* ����VAL */
				TIM_SetCounter(TIM3,0);	/* ����CNT */
				TIM3CH1_CAPTURE_STA|=0X40;	/* ��־Ϊ�Ѿ���׽����� */
			}            
		}                                                
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update);
}

/**
 *@function TIM4 PWM ��ʼ�� 
 *@param 
 *			arr���Զ���װֵ
 *			psc��ʱ��Ԥ��Ƶ��
 */
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
   /* ��ʼ��TIM4 */
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* ��ʼ��TIM4 PWMģʽ */	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2 */
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* �������:TIM����Ƚϼ��Ը� */
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC1 */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC2 */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC3 */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  /* ��ʼ������TIM4 OC4 */

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  /* ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ��� */
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
}
