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

int main(void)

{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	uart_init(115200);
	motor_init();
	
	C_S.Vx = 0;
	C_S.Vy = 0;
	C_S.W = 5
	
	
	;
		speed_inverse_solution();
		motor_run();    
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}

