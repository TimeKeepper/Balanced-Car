#ifndef PID_H
#define PID_H

#include "system.h"

typedef struct
{
    float Kp;		  
    float Ki;	       
    float Kd;		      
    float dt;	      
    float max;	         
    float min;	            
    float setPoint;		 
    float integral;		    
    float pre_error;		
} PID;


void PID_Init(PID *pid, float kp, float ki, float kd, float dt, float max, float min,float SetPoint);

float PID_Calc(PID *pid, float NextPoint);

void PID_Set(PID *pid, float p, float i, float d);

void PID_SetSetPoint(PID *pid, float setPoint);

void PID_SetOutputLimit(PID *pid, float min, float max);
#endif
