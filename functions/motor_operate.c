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

#include "sys_conf.h"

void motor_init(void)
{
	TIM3_Init(1,1);
}

void motor_run(wheel_speed* ws)
{
	
}
