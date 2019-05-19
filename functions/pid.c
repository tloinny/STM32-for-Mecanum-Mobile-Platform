#include "pid.h"

float kp = 0;
float ki = 0;
float kd = 0;
float Goal = 0;
float Output = 0;
float PTerm = 0;
float ITerm = 0;
float DTerm = 0;
float LastError = 0;
float WindupGuard = 0;
float SampleTime = 0;
int CurrentTime = 0;
int LastTime = 0;
	
/**
 *@function
 *@param
 *@return
 */
void PID_CONTROLLER(float Kp, float Ki, float Kd, float s_t, float w_g)
{
}

/**
 *@function
 *@param
 *@return
 */
void setPID(float kp, float ki, float kd)
{

}

/**
 *@function
 *@param
 *@return
 */
void setWindup(float windup)
{

}

/**
 *@function
 *@param
 *@return
 */
void setSampleTime(float sample_time)
{

}

/**
 *@function
 *@param
 *@return
 */
float getKp()
{

}

/**
 *@function
 *@param
 *@return
 */
float getKi()
{

}

/**
 *@function
 *@param
 *@return
 */
float getKd()
{

}

/**
 *@function
 *@param
 *@return
 */
float getWindup()
{

}

/**
 *@function
 *@param
 *@return
 */
void clear()
{

}

/**
 *@function
 *@param
 *@return
 */
void update(float FeedbackValue)
{

}
