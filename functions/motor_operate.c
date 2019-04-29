/**
 *@title Mecanum Mobile Platform firmware config file
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	电机驱动
 */

#include "motor_operate.h"
extern wheel_speed W_S;
/**
 *@function 电机资源初始化
 *@param void
 *@return void
 */
void motor_init(void)
{
	motor_io_init();	/* 初始化电机驱动引脚 */
	encoder_io_init();	/* 初始化编码器接口 */
	TIM3_Init(TIM3_ARR,TIM3_PSC);	/* 初始化TIM3,提供给编码器使用 */
	TIM4_PWM_Init(TIM4_ARR,TIM4_PSC);	/* 初始化TIM4,提供给电机驱动使用 */
}

/**
 *@function 电机运动
 *@param void
 *@return void
 */
void motor_run()
{
	/* 先设置转向 */
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
	
	/* 再设置转速 */
	TIM_SetCompare1(TIM4,TIM4_ARR*(W_S.Vw0)/MAX_RPM);
	TIM_SetCompare2(TIM4,TIM4_ARR*(W_S.Vw1)/MAX_RPM);
	TIM_SetCompare3(TIM4,TIM4_ARR*(W_S.Vw2)/MAX_RPM);
	TIM_SetCompare4(TIM4,TIM4_ARR*(W_S.Vw3)/MAX_RPM);
}

/**
 *@function 设置电机转向
 *@param 
 *			m 	目标电机
 *			dir 转向
 *@return void
 */
void set_dir(int m, char dir)
{
	if(dir == 1)	/* 正转 */
	{		
		GPIO_ResetBits(Motors[m].motor_gpio_type,Motors[m].motor_gpio[1].GPIO_Pin);	/* N1 0*/
		GPIO_SetBits(Motors[m].motor_gpio_type,Motors[m].motor_gpio[2].GPIO_Pin);	/* N2 1*/
	}else if(dir == 0)	/* 反转 */
	{
		GPIO_SetBits(Motors[m].motor_gpio_type,Motors[m].motor_gpio[1].GPIO_Pin);	/* N1 1*/	
		GPIO_ResetBits(Motors[m].motor_gpio_type,Motors[m].motor_gpio[2].GPIO_Pin);	/* N2 0*/		
	}else	/* 停转 */
	{
		GPIO_ResetBits(Motors[m].motor_gpio_type,Motors[m].motor_gpio[1].GPIO_Pin);	/* N1 0*/	
		GPIO_ResetBits(Motors[m].motor_gpio_type,Motors[m].motor_gpio[2].GPIO_Pin);	/* N2 0*/			
	}
}
