#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "system.h"

//IO方向设置
//此处！！！一定一定要注意！！！根据IO口设定高寄存器和低寄存器以及对应的32位数字！！！
#define MPU_SDA_IN()  {MPU_IIC_SDA_PORT->CRH&=0XFFF0FFFF;MPU_IIC_SDA_PORT->CRH|=8<<16;}
#define MPU_SDA_OUT() {MPU_IIC_SDA_PORT->CRH&=0XFFF0FFFF;MPU_IIC_SDA_PORT->CRH|=3<<16;}

//IO操作函数	 
#define MPU_IIC_SCL_PORT	GPIOA
#define MPU_IIC_SCL_Pin		GPIO_Pin_11
#define MPU_IIC_SDA_PORT	GPIOA
#define MPU_IIC_SDA_Pin		GPIO_Pin_12
#define MPU_IIC_SCL    PAout(11) 		//SCL
#define MPU_IIC_SDA    PAout(12) 		//SDA	 
#define MPU_READ_SDA   PAin(12) 		//输入SDA 

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif










