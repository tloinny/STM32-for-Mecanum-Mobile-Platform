/**
 *@title Mecanum Mobile Platform firmware config file
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	�������
 */

#include "motor_operate.h"
extern wheel_speed W_S;

/**
 *@function �����Դ��ʼ��
 *@param void
 *@return void
 */
void motor_init(void)
{
	TIM3_Init(TIM3_ARR,TIM3_PSC);	/* ��ʼ��TIM3,�ṩ��������ʹ�� */
	TIM4_PWM_Init(TIM4_ARR,TIM4_PSC);	/* ��ʼ��TIM4,�ṩ���������ʹ�� */
	motor_enable();
}

/**
 *@function ����˶�
 *@param void
 *@return void
 */
void motor_run()
{
	/* ������ת�� */
	if(W_S.Vw0 > 0)
	{
		set_dir(0,1);
	}else
	{
		set_dir(0,0);
	}
	
	if(W_S.Vw1  > 0)
	{
		set_dir(1,1);
	}else
	{
		set_dir(1,0);
	}
	
	if(W_S.Vw2 > 0)
	{
		set_dir(2,1);
	}else
	{
		set_dir(2,0);
	}
	
	if(W_S.Vw3 > 0)
	{
		set_dir(3,1);
	}else
	{
		set_dir(3,0);
	}
	
	/* ������ת�� */
	TIM_SetCompare1(TIM4,TIM4_ARR*(W_S.Vw0)/MAX_RPM);
	TIM_SetCompare2(TIM4,TIM4_ARR*(W_S.Vw1)/MAX_RPM);
	TIM_SetCompare3(TIM4,TIM4_ARR*(W_S.Vw2)/MAX_RPM);
	TIM_SetCompare4(TIM4,TIM4_ARR*(W_S.Vw3)/MAX_RPM);
}

/**
 *@function ���õ��ת��
 *@param 
 *			m 	Ŀ����
 *			dir ת��
 *@return void
 */
void set_dir(int m, char dir)
{
	if(dir == 1)	/* ��ת */
	{		
		GPIO_ResetBits(Motors[m].motor_gpio_type[1],Motors[m].motor_gpio[1].GPIO_Pin);	/* N1 0*/
		GPIO_SetBits(Motors[m].motor_gpio_type[2],Motors[m].motor_gpio[2].GPIO_Pin);	/* N2 1*/
	}else if(dir == 0)	/* ��ת */
	{
		GPIO_SetBits(Motors[m].motor_gpio_type[1],Motors[m].motor_gpio[1].GPIO_Pin);	/* N1 1*/	
		GPIO_ResetBits(Motors[m].motor_gpio_type[2],Motors[m].motor_gpio[2].GPIO_Pin);	/* N2 0*/		
	}else	/* ͣת */
	{
		GPIO_ResetBits(Motors[m].motor_gpio_type[1],Motors[m].motor_gpio[1].GPIO_Pin);	/* N1 0*/	
		GPIO_ResetBits(Motors[m].motor_gpio_type[2],Motors[m].motor_gpio[2].GPIO_Pin);	/* N2 0*/			
	}
}

void motor_enable(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

/**
 *@function �����Դ��ʼ��
 *@param void
 *@return void
 */
int read_speed(int motor, int wait_time, int times)
{
	int TEMP;
	switch(motor)
	{
		case 1:
			TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
			scan = 1;
				for (int i = 0; i < times; ++i)
				{
					if(wait_time == waiting_forever)
					{
						while(!(TIM3CH1_CAPTURE_STA&0X80));
						temp0=TIM3CH1_CAPTURE_STA&0X3F; 
						temp0*=65536;
						temp0+=TIM3CH1_CAPTURE_VAL;
					}else
					{
						int n = 0;
						for(;n < wait_time; ++n)
						{
							if(TIM3CH1_CAPTURE_STA&0X80)
							{
								temp0=TIM3CH1_CAPTURE_STA&0X3F; 
								temp0*=65536;
								temp0+=TIM3CH1_CAPTURE_VAL;
								break;
							}
							delay_us(1);
						}
						if(n > wait_time) return error;
					}

					if (temp0 >= 5000)	/* ��Χ�ж�,�����쳣��ֵ */
					{
						TEMP += temp0;
					}else
					{
						--i;	/* ����һ�� */
					}
				}
			TEMP /= times;
			rpm0 = 6000000/(2*TEMP*9);
			TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
			return rpm0;
			break;
		case 2:
			TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
			scan = 2;
				for (int i = 0; i < times; ++i)
				{
					if(wait_time == waiting_forever)
					{
						while(!(TIM3CH2_CAPTURE_STA&0X80));
						temp1=TIM3CH2_CAPTURE_STA&0X3F; 
						temp1*=65536;
						temp1+=TIM3CH2_CAPTURE_VAL;
					}else
					{
						int n = 0;
						for(;n < wait_time; ++n)
						{
							if(TIM3CH2_CAPTURE_STA&0X80)
							{
								temp1=TIM3CH2_CAPTURE_STA&0X3F; 
								temp1*=65536;
								temp1+=TIM3CH2_CAPTURE_VAL;
								break;
							}
							delay_us(1);
						}
						if(n > wait_time) return error;
					}
					
					if (temp1 >= 5000)	/* ��Χ�ж�,�����쳣��ֵ */
					{
						TEMP += temp1;
					}else
					{
						--i;	/* ����һ�� */
					}
				}
			TEMP /= times;
			rpm1 = 6000000/(2*TEMP*9);
			TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
			return rpm1;
			break;
		case 3:
			TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
			scan = 3;
				for (int i = 0; i < times; ++i)
				{
					if(wait_time == waiting_forever)
					{
						while(!(TIM3CH3_CAPTURE_STA&0X80));
						temp2=TIM3CH3_CAPTURE_STA&0X3F; 
						temp2*=65536;
						temp2+=TIM3CH3_CAPTURE_VAL;
					}else
					{
						int n = 0;
						for(;n < wait_time; ++n)
						{
							if(TIM3CH3_CAPTURE_STA&0X80)
							{
								temp2=TIM3CH3_CAPTURE_STA&0X3F; 
								temp2*=65536;
								temp2+=TIM3CH3_CAPTURE_VAL;
								break;
							}
							delay_us(1);
						}
						if(n > wait_time) return error;
					}
					
					if (temp2 >= 5000)	/* ��Χ�ж�,�����쳣��ֵ */
					{
						TEMP += temp2;
					}else
					{
						--i;	/* ����һ�� */
					}
				}
			TEMP /= times;
			rpm2 = 6000000/(2*TEMP*9);
			TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
			return rpm2;
			break;
		case 4:
			TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
			scan = 4;
				for (int i = 0; i < times; ++i)
				{
					if(wait_time == waiting_forever)
					{
						while(!(TIM3CH4_CAPTURE_STA&0X80));
						temp3=TIM3CH4_CAPTURE_STA&0X3F; 
						temp3*=65536;
						temp3+=TIM3CH4_CAPTURE_VAL;
					}else
					{
						int n = 0;
						for(;n < wait_time; ++n)
						{
							if(TIM3CH4_CAPTURE_STA&0X80)
							{
								temp3=TIM3CH4_CAPTURE_STA&0X3F; 
								temp3*=65536;
								temp3+=TIM3CH4_CAPTURE_VAL;
								break;
							}
							delay_us(1);
						}
						if(n > wait_time) return error;
					}
					
					if (temp3 >= 5000)	/* ��Χ�ж�,�����쳣��ֵ */
					{
						TEMP += temp3;
					}else
					{
						--i;	/* ����һ�� */
					}
				}
			TEMP /= times;
			rpm3 = 6000000/(2*TEMP*9);
			TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);
			return rpm3;
			break;
		default:
			break;
	}
	return 0;
}