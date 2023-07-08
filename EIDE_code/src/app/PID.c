#include "PID.h"

void PID_Init(PID *pid, float kp, float ki, float kd, float dt, float max, float min,float SetPoint)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->dt = dt;
    pid->max = max;
    pid->min = min;
    pid->setPoint = SetPoint;
    pid->integral = 0;
    pid->pre_error = 0;
}

float PID_Calc(PID *pid, float NextPoint){
    float error = pid->setPoint - NextPoint;
    pid->integral += error * pid->dt;
    float derivative = (error - pid->pre_error) / pid->dt;
    float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
    if(output > pid->max)
        output = pid->max;
    else if(output < pid->min)
        output = pid->min;
    pid->pre_error = error;
    return output;
}

void PID_Set(PID *pid, float p, float i, float d){
    pid->Kp = p;
    pid->Ki = i;
    pid->Kd = d;
}

void PID_SetSetPoint(PID *pid, float setPoint){
    pid->setPoint = setPoint;
}

void PID_SetOutputLimit(PID *pid, float min, float max){
    pid->min = min;
    pid->max = max;
}