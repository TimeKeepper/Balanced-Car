#ifndef _PID_H_
#define _PID_H_

#include "system.h"

extern double Med_of_Mec;			//机械中值(6.9)-5.4
extern double kp;  //(400)336
extern double kd;  //(120)120,175? //平衡模式下120，遥控模式下90，载重模式下140
extern double ki; 
extern double kdd; 
extern double VKp;//120		//这个系数控制的是保持速度为相对0的倾向
extern double VKi;//0.6			//这个系数控制的是保持位移为0，也就是在扰动后依旧回归初始位置的执念
extern double Yp;
extern double Yd; 
extern double Dp;
extern double Dd;

extern short pre_gyroy;
extern float lastime_pitch;
extern float Control_Out_Left;
extern float Control_Out_Right;
extern float last_filt_velocity;
extern float velocity_sum;
extern float angel_sum;
extern float last_Yaw_angle;
extern float expect_Yaw_angel;
 
/*
传入参数 Angle_y 			y轴倾角
Gyro_y				y轴角速度
Expect_Angle_y 		y轴期望倾角
*/

void Balance_Control(float Angle_y,short Gyro_y,float Expect_Angle_y);
void velocity_PID_value(int velocity);
void Yaw_PID_value(float Yaw,float Expect_Angle_Yaw);
float Yaw_culculating(float d);
void PID_parameter_config(u8 mode);
float distance_PID_culculating(float current,float last);

#endif
