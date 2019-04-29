#ifndef __TIMER_H
#define __TIMER_H
#include "sys_conf.h"

void TIM3_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
void TIM4_PWM_Init(u16 arr,u16 psc);

#endif
