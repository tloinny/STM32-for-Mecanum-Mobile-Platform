#ifndef _MOTOR_OPERATE_H_
#define _MOTOR_OPERATE_H_
#include "sys_conf.h"

void motor_init(void);
void motor_run(void);
void set_dir(int m, char dir);
void motor_enable(void);
int read_speed(int motor, int wait_time, int times);
#endif
