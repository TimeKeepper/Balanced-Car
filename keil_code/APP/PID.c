#include "PID.h"

float Control_Out_Left;
float Control_Out_Right;
float last_filt_velocity;
float velocity_sum = 0;
float angel_sum = 0;
float last_Yaw_angle = 0;
float expect_Yaw_angel = 0;
float lastime_pitch = 0;
short pre_gyroy = 0;

double Med_of_Mec = 5.5;
double kp  = 270;  //(450)270
double kd = 125;  //(140)84,175? //平衡模式下120，遥控模式下90，载重模式下140
double ki = 0;
double VKp = 95;//140		//这个系数控制的是保持速度为相对0的倾向
double VKi = 0.475;//0.7			//这个系数控制的是保持位移为0，也就是在扰动后依旧回归初始位置的执念
double Yp = 40;
double Yd = 0;
double Dp = 1.4;
double Dd = 0.5;

void Balance_Control(float Angle_y,short Gyro_y,float Expect_Angle_y){
	float error_y;
	float balance_y;
	
	float a = 0;pre_gyroy*=a;
	pre_gyroy = Gyro_y;
	
	error_y = Angle_y - Expect_Angle_y;	//这两个变量哪个先哪个后以后再说，测试时就知道了
	angel_sum += Angle_y - Expect_Angle_y;
	
	//PD控制，由于角速度是角度的一阶倒数，所以满足pid公式中d项要求，不用除dt，直接乘dt就行了。
	if(angel_sum >100) angel_sum = 100;
	else if(angel_sum < -100) angel_sum = -100;
	balance_y = kp * error_y + kd * Gyro_y + ki * angel_sum;//这个d前面的符号的正负也以后再说
	lastime_pitch = error_y;
	
	//赋值给控制输出变量
	Control_Out_Left = balance_y;
	Control_Out_Right = balance_y;
}

void velocity_PID_value(int velocity)
{
	float a=0.3;       	//滤波系数（反映滤波程度）
	float filt_velocity;
	filt_velocity = a*velocity + (1-a)*last_filt_velocity; //一阶速度滤波
	velocity_sum += filt_velocity;
	if(velocity_sum>6000) velocity_sum =6000;  //当与初始位置的距离超过1000，就不要再有执念了
	else if(velocity_sum<-6000) velocity_sum = -6000;
	last_filt_velocity = filt_velocity;                    //此次速度记录为“上次速度”

	Control_Out_Left -= VKp*filt_velocity + VKi*velocity_sum;
	Control_Out_Right -= VKp*filt_velocity + VKi*velocity_sum;
}

float Yaw_culculating(float d){
	if(d>180||d<-180){
		if(d>0) return d-360;
		else return 360-d;
	}
	return d;
}


void Yaw_PID_value(float Yaw,float Expect_Angle_Yaw){
	float error_y;
	float balance_y;
	
	error_y = Yaw_culculating(Yaw-Expect_Angle_Yaw);
	last_Yaw_angle = error_y;
	balance_y = Yp*error_y + Yd*last_Yaw_angle;
	Control_Out_Left -= balance_y;
	Control_Out_Right += balance_y;
}

float distance_PID_culculating(float current,float last){
	float result = Dp*(current-60)+Dd*(last-60);
	if(result >75) result = 75;
	else if(result < -75) result = -75;
	return result;
}

void PID_parameter_config(u8 mode){
	if(mode == 1){
		kp = 270;//270
		kd = 110;//110
		ki = 0;
		VKp = 90;//90
		VKi = 0.45;//0.45
		Yp = 40;//40
	}
	else if(mode == 2){
		kp = 270;//300
		kd = 110; //80
		ki = 0;
		VKp = 90;//80
		VKi = 0.45;//0.4
		Yp = 40;//60
	}
	else if(mode == 3){
		kp = 300;//300
		kd = 150;
		ki = 0;
		VKp = 90;
		VKi = 0.45;
	}
}
