#include "usart.h"

int uart_message = 56;

int fputc(int ch,FILE *p)  	//是更底层的输出函数，printf的实现就调用了这个函数
{							//所以，想要实现串口printf的重新定义，只需要对fputc函数进行重定义就能做到
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

void USART1_Init(u32 pound)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//初始化GPIO结构体
	USART_InitTypeDef USART_InitStructure;	//初始化串口通信结构体
	NVIC_InitTypeDef NVIC_InitStructure;	//初始化中断管理结构体
	
	//时钟使能
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(USART1_TX_PORT),ENABLE);
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(USART1_RX_PORT),ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//GPIO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = USART1_TX_Pin;	//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = USART1_RX_Pin; 	//接收模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入(也可以选择上拉输入，详见中文参考手册p110)
	GPIO_Init(USART1_RX_PORT,&GPIO_InitStructure);
	
	//USART配置
	USART_InitStructure.USART_BaudRate = pound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Odd;	//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 		//收发模式
	USART_Init(USART1, &USART_InitStructure); 			//初始化串口1
		
	USART_Cmd(USART1,ENABLE); 							//串口使能
	/*TC即transmit compelete,也就是发送完成的意思，TXE即Tx DR Empty，发送寄存器空了*/
	/* 有一个形象的描述USART_FLAG_TC和USART_FLAG_TXE的比喻
	    USART_FLAG_TC为真时的含义是“枪膛”空了，USART_FLAG_TXE的含义是“弹仓”空了
		当我们通过串口发送信息时，先将数据传输到弹仓，硬件将数据转入枪膛中
		此时弹仓空了，枪膛不空，因为TX正在传输数据，“子弹”还未射出
		因此此时TC为0，枪膛不空，TXE为1，弹仓空了
		此时我们还可以写入数据到弹仓中，由于TX还在发送数据，此时TC为0，TXE为0
		等到数据发送完毕，若又没有装入新的数据，则TC为0，TXE为1*/
	USART_ClearFlag(USART1,USART_FLAG_TC);				//清除串口标志位
	/*USART_IT_TC即发送完成后进入中断，和51的方式一样
	  USART_IT_TXE即发送寄存器为空进入中断
	  USART_IT_IDLE即受到一帧数据进入中断
	  而此处采用的USART_IT_RXNE即受到一帧数据后进入中断*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		//开启相关中断
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	//中断通道设置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//中断使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//中断优先级设置
	NVIC_Init(&NVIC_InitStructure);

}

//该中断函数支持接收一串字符串，必须以回车表明一次传输的结束
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){
		uart_message =USART_ReceiveData(USART1);
		if(uart_message==58){}
	}
}
