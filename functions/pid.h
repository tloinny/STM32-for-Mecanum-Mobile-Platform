#ifndef _PID_H_
#define _PID_H_
#include "sys_conf.h"

void PID_CONTROLLER(float Kp, float Ki, float Kd, float s_t, float w_g);
void setPID(float kp, float ki, float kd);
void setWindup(float windup);
void setSampleTime(float sample_time);
float getKp(void);
float getKi(void);
float getKd(void);
float getWindup(void);
void clear(void);
void update(float FeedbackValue);

#endif
