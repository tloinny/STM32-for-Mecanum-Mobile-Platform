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
	uart2_init(115200);
	motor_init();
	LED0 = 1;
	
	C_S.Vx = 50;
	C_S.Vy = 500;
	C_S.W = 5;
		speed_inverse_solution();
		motor_run();
		u32 temp0 = 0;
	u32 temp1 = 0;
	u32 temp2 = 0;
	u32 temp3 = 0;
	while(1)
	{
		if(TIM3CH1_CAPTURE_STA&0X80)
		{
			temp0=TIM3CH1_CAPTURE_STA&0X3F; 
			temp0*=65536;
			temp0+=TIM3CH1_CAPTURE_VAL;
			printf("-------------------\r\n"); 
			printf("CH1:%d us\r\n",temp0);
			TIM3CH1_CAPTURE_STA=0;
		}
		if(TIM3CH2_CAPTURE_STA&0X80)
		{
			temp1=TIM3CH2_CAPTURE_STA&0X3F; 
			temp1*=65536;
			temp1+=TIM3CH2_CAPTURE_VAL;
			printf("CH2:%d us\r\n",temp1); 
			TIM3CH2_CAPTURE_STA=0;
		}
		if(TIM3CH3_CAPTURE_STA&0X80)
		{
			temp2=TIM3CH3_CAPTURE_STA&0X3F; 
			temp2*=65536;
			temp2+=TIM3CH3_CAPTURE_VAL;
			printf("CH3:%d us\r\n",temp2); 
			TIM3CH3_CAPTURE_STA=0;
			temp2 = 0;
		}
		if(TIM3CH4_CAPTURE_STA&0X80)
		{
			temp3=TIM3CH4_CAPTURE_STA&0X3F; 
			temp3*=65536;
			temp3+=TIM3CH4_CAPTURE_VAL;
			printf("CH4:%d us\r\n",temp3);
			printf("-------------------\r\n"); 			
			TIM3CH4_CAPTURE_STA=0;
			temp3 = 0;
		}		
	}
}

