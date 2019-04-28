/**
 *@title Mecanum Mobile Platform firmware config file
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-4-28  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	麦克纳姆轮移动平台运动学逆解
 */
#include "inverse_solution.h"

wheel_speed W_S;

void speed_inverse_solution(car_speed* car_s)
{
	W_S.Vw0 = car_s->Vx - car_s->Vy - car_s->W *(a+b);
	W_S.Vw1 = car_s->Vx + car_s->Vy + car_s->W *(a+b);
	W_S.Vw2 = car_s->Vx - car_s->Vy + car_s->W *(a+b);
	W_S.Vw3 = car_s->Vx + car_s->Vy - car_s->W *(a+b);
}
