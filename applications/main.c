/**
 *@title Mecanum Mobile Platform Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Mecanum-Mobile-Platform]
 *
 *@date on 2019-4-24  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Mecanum Mobile Platform application
 */
 
#include "sys_conf.h"

u32 temp0 = 0;
u32 temp1 = 0;
u32 temp2 = 0;
u32 temp3 = 0;
u32 rpm0 = 0;
u32 rpm1 = 0;
u32 rpm2 = 0;
u32 rpm3 = 0;

int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	uart2_init(115200);
	motor_init();
	LED0 = 1;
	
	C_S.Vx = 50;
	C_S.Vy = 500;
	C_S.W = 5;
		speed_inverse_solution();
		motor_run();
		TIM_SetCompare1(TIM4,100);
	TIM_SetCompare2(TIM4,100);
	TIM_SetCompare3(TIM4,100);
	TIM_SetCompare4(TIM4,100);
//	u32 temp0 = 0;
//	u32 temp1 = 0;
//	u32 temp2 = 0;
//	u32 temp3 = 0;
	while(1)
	{
		int num = 1;
		printf("rpm%d:%d \r\n",num,read_speed(num,waiting_forever,2));
	}
}

