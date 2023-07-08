#include "system.h"
#include "led.h"
#include "OLED.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "hwyk.h"
#include "PID.h"
#include "hsr04.h"
#include "Motor.h"
#include "usart.h"

#define default_filiter 0

void ALL_Init(void);
int sign_for_load = 0;
void OLED_SHOW_message(void);
void loop_clear(void);
void control_stand_by(void);
void control_turn_on(float angle,float V,float S);
void control_turn_back(float angle,float V,float S);
void control_turn_left(void);
void control_turn_right(void);


float pitch,roll,yaw; 		
short aacx,aacy,aacz;		
short gyrox,gyroy,gyroz;	
int expect_Yaw_counter = 0;

u16 speed_accound = 3;
int speed_recover_filiter=0;
u32 pre_sign = 0;
int velocity;

int main(){
	USART1_Init(38400);
	ALL_Init();
	OLED_ShowString(1,1,"OK");
	float pitch_load = 0;
	loop: while(1)
	{	
//		velocity = ((Encoder1_Get()-Encoder2_Get())/2);
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	
		}
//		if(exti_sign == 1){
//				IR_RECEIVE[3]=gGUA_InfraredReceiver_Data%256;
//				IR_RECEIVE[2]=(gGUA_InfraredReceiver_Data>>8)%256;
//				IR_RECEIVE[1]=(gGUA_InfraredReceiver_Data>>16)%256;
//				IR_RECEIVE[0]=(gGUA_InfraredReceiver_Data>>24)%256;
//				exti_sign = 0;
//			}
		if((pitch_load<pitch-Med_of_Mec)||1){
			pitch_load = pitch-Med_of_Mec;
			OLED_ShowSignedNum(1,12,(pitch_load)*10,3);
		}
		if(uart_message==56){Motor_SetSpeed1(0);Motor_SetSpeed2(0);expect_Yaw_angel = yaw;velocity_sum=0;Encoder1_Get();Encoder2_Get();}
		else if(sign>11){
			if(pre_sign){pre_sign--;loop_clear();goto loop;}
			if(uart_message==49){
				PID_parameter_config(2);if(speed_recover_filiter<default_filiter) speed_recover_filiter++;
				control_turn_on(0.75*speed_accound,3*speed_accound,400*speed_accound);//0.75,3,400
			}
			else if(uart_message==50){
				PID_parameter_config(2);if(speed_recover_filiter>-default_filiter) speed_recover_filiter--;
				control_turn_back(0.75*speed_accound,3*speed_accound,400*speed_accound);
			}
			else if(uart_message==51){//右
				speed_recover_filiter = 0;
				PID_parameter_config(1);
				control_turn_right();
				expect_Yaw_angel = yaw;
			}
			else if(uart_message==52){//左
				speed_recover_filiter = 0;
				PID_parameter_config(1);
				control_turn_left();
				expect_Yaw_angel = yaw;
			}
			else{
				if(speed_recover_filiter>0){PID_parameter_config(2);
				control_turn_on(0.75*speed_accound,3*speed_accound,400*speed_accound);
				speed_recover_filiter--;}
				else if(speed_recover_filiter<0){PID_parameter_config(2);
				control_turn_on(0.75*speed_accound,3*speed_accound,400*speed_accound);
				speed_recover_filiter++;}
				else{
					if(uart_message == 59)PID_parameter_config(3);else PID_parameter_config(1);
					float current_parameter = distance_PID_culculating(current_distant,lastime_distant);
					if(uart_message == 57&&current_distant<120) {control_turn_on(0.05*current_parameter,0.15*current_parameter,0);}
					else{
						switch(uart_message){
							case 162:speed_accound = 1;break;
							case 98:speed_accound = 2;break;
							case 53:speed_accound = 3;break;
							case 34:speed_accound = 4;break;
							case 2:speed_accound = 5;break;
							case 54:speed_accound = 4;break;
							case 224:speed_accound = 7;break;
							case 168:speed_accound = 8;break;
							case 55:speed_accound = 5;break;
							default:break;
						}
		//				if(current_distant > 70&&current_distant<110) control_turn_on((40-(110-current_distant))/20,((40-(110-current_distant))*10)/20,((40-(110-current_distant))*600)/20);
		//				else if(current_distant < 40) control_turn_back((40-current_distant)/20,((40-current_distant)*10)/20,((40-current_distant)*600)/20);
		//				else control_stand_by();
						control_stand_by();	
					}
				}					
			}
			HSR04_Trig_Open();
//			if(uart_message == '1') expect_Yaw_counter=10;
//			else if(uart_message == '2') expect_Yaw_counter=-10;
//			if(expect_Yaw_counter>0) {Yaw_PID_value(yaw,yaw + 90);expect_Yaw_counter--;if(expect_Yaw_counter == 0) expect_Yaw_angel = yaw;}
//			else if(expect_Yaw_counter<0) {Yaw_PID_value(yaw,yaw - 90);expect_Yaw_counter++;if(expect_Yaw_counter == 0) expect_Yaw_angel = yaw;}
//			
			
			

//			if(sign_for_load>10) {Motor_SetSpeed1(0);Motor_SetSpeed2(0);while(1);}//防止发癫
			
			if(pitch>60||pitch<-60) sign_for_load++;
			else sign_for_load = 0;
			
			Motor_SetSpeed1((int)(Control_Out_Left));
			Motor_SetSpeed2((int)(Control_Out_Right));
			
			
			OLED_SHOW_message();
			loop_clear();
		}
	}
}

void control_stand_by(void){
	Balance_Control(pitch,((float)gyroy)/100,Med_of_Mec);
	velocity_PID_value((Encoder1_Get()-Encoder2_Get())/2);
	Yaw_PID_value(yaw,expect_Yaw_angel);
}

void control_turn_on(float angle,float V,float S){
	Balance_Control(pitch,((float)gyroy)/100,angle+Med_of_Mec);
	velocity_PID_value(V+((Encoder1_Get()-Encoder2_Get())/2));
	Yaw_PID_value(yaw,expect_Yaw_angel);
	velocity_sum = S;
}

void control_turn_back(float angle,float V,float S){
	Balance_Control(pitch,((float)gyroy)/100,-angle+Med_of_Mec);
	velocity_PID_value(-V+((Encoder1_Get()-Encoder2_Get())/2));
	Yaw_PID_value(yaw,expect_Yaw_angel);
	velocity_sum = -S;
}

void control_turn_left(void){
	Balance_Control(pitch,((float)gyroy)/100,Med_of_Mec);
	velocity_PID_value(((Encoder1_Get()-Encoder2_Get())/2));
	Control_Out_Left+=1000;
	Control_Out_Right-=1000;
}

void control_turn_right(void){
	Balance_Control(pitch,((float)gyroy)/100,Med_of_Mec);
	velocity_PID_value(((Encoder1_Get()-Encoder2_Get())/2));
	Control_Out_Left-=1000;
	Control_Out_Right+=1000;
}

void ALL_Init(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Systick_Init(72);
	OLED_Init();
	LED_Init(T_LEDS1);
	GUA_Infrared_Receiver_Init();
	while(mpu_dmp_init()){
		delay_ms(10);
	}
	Motor_Init();
	Motor_SetSpeed2(0);
	Motor_SetSpeed1(0);
	Encoder_Init();
	HSR04_Init();
}

void OLED_SHOW_message(void){
	OLED_ShowNum(1,1,current_distant,3);
//	OLED_ShowSignedNum(1,1,yaw,3);

	OLED_ShowSignedNum(2,1,uart_message,8);
//		OLED_ShowSignedNum(2,1,velocity_sum,4);
//	OLED_ShowChar(2,1,uart_message);
	OLED_ShowSignedNum(3,1,Encoder1_Get(),6);
	OLED_ShowSignedNum(4,1,-Encoder2_Get(),6);
	// OLED_ShowSignedNum(3,1,(int)(velocity_sum),6);
	// OLED_ShowSignedNum(4,1,(int)(Control_Out_Right),6);
}

void loop_clear(void){
	if(!runtime_account--){
		if(IR_RECEIVE[2]==90||IR_RECEIVE[2]==16){
			expect_Yaw_angel = yaw;
		}
		IR_RECEIVE[2] = 0;
	}
	sign = 0;
//	uart_message = 0;
}


/*
����repeat��⺯���������������ڸ�Ϊ110ms����11ms��ӭ��repeat��һ����10�ݼ��ı�־λ���ڱ�־λ����0ʱ����
֮ǰ��״̬������ǰ�������ˣ�ֱ������ת�䣩
*/

//float pitch,roll,yaw; 		//ŷ����
//short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
//short gyrox,gyroy,gyroz;	//������ԭʼ����
//short temp;					//�¶�

//while(mpu_dmp_init()){
//	delay_ms(20);
//}

//if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//{
//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�������
//}
//OLED_ShowSignedNum(2,1,pitch,3);



