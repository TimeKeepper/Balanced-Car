#include "Mine_bno055.h"

static GPIO_TypeDef* LOC_SCL_PORT = 0;
static uint16_t LOC_SCL_PIN = 0;
static GPIO_TypeDef* LOC_SDA_PORT = 0;
static uint16_t LOC_SDA_PIN = 0;

#define SDA_OUT()   {LOC_SDA_PORT->CRH&=0XFF0FFFFF;LOC_SDA_PORT->CRH|=8<<20;}//PB13
#define SDA_IN()    {LOC_SDA_PORT->CRH&=0XFF0FFFFF;LOC_SDA_PORT->CRH|=3<<20;}
#define SDA_W()    PBout(13)
#define SDA_R()    PBin(13)
#define SCL_W()    PBout(12)

#define IOIIC_Delay()   delay_us(2)

void IOIIC_Init(GPIO_TypeDef* SCL_PORt,uint16_t SCL_PIN,GPIO_TypeDef* SDA_PORT,uint16_t SDA_PIN){
    LOC_SCL_PORT = SCL_PORt;
    LOC_SCL_PIN = SCL_PIN;
    LOC_SDA_PORT = SDA_PORT;
    LOC_SDA_PIN = SDA_PIN;
    GPIO_RCC_ENABLE(SCL_PORt);
    GPIO_RCC_ENABLE(SDA_PORT);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCL_PORt,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SDA_PORT,&GPIO_InitStructure);

    SDA_W() = 1;
    SCL_W() = 1;
}

void IOIIC_Start(void){
    SDA_OUT();
    SDA_W() = 1;
    SCL_W() = 1;
    IOIIC_Delay();
    SDA_W() = 0;
    IOIIC_Delay();
    SCL_W() = 0;
}

void IOIIC_Stop(void){
    SDA_OUT();
    SCL_W() = 0;
    SDA_W() = 0;
    IOIIC_Delay();
    SCL_W() = 1;
    SDA_W() = 1;
    IOIIC_Delay();
}

u8 IOIIC_Wait_Ack(void){
    u8 ucErrTime=0;
    SDA_IN();
    SDA_W() = 1;
    IOIIC_Delay();
    SCL_W() = 1;
    IOIIC_Delay();
    while(SDA_R()){
        ucErrTime++;
        if(ucErrTime>250){
            IOIIC_Stop();
            return 1;
        }
    }
    SCL_W() = 0;
    return 0;
}

void IOIIC_Ack(void){
    SCL_W() = 0;
    SDA_OUT();
    SDA_W() = 0;
    IOIIC_Delay();
    SCL_W() = 1;
    IOIIC_Delay();
    SCL_W() = 0;
}

void IOIIC_NAck(void){
    SCL_W() = 0;
    SDA_OUT();
    SDA_W() = 1;
    IOIIC_Delay();
    SCL_W() = 1;
    IOIIC_Delay();
    SCL_W() = 0;
}

void IOIIC_Send_Byte(u8 txd){
    u8 t;
    SDA_OUT();
    SCL_W() = 0;
    for(t=0;t<8;t++){
        SDA_W() = (txd&0x80)>>7;
        txd<<=1;
        IOIIC_Delay();
        SCL_W() = 1;
        IOIIC_Delay();
        SCL_W() = 0;
        IOIIC_Delay();
    }
}

u8 IOIIC_Read_Byte(unsigned char ack){
    unsigned char i,receive=0;
    SDA_IN();
    for(i=0;i<8;i++){
        SCL_W() = 0;
        IOIIC_Delay();
        SCL_W() = 1;
        receive<<=1;
        if(SDA_R())receive++;
        IOIIC_Delay();
    }
    if (!ack)IOIIC_NAck();
    else IOIIC_Ack();
    return receive;
}

s8 I2C_WRITE_STRING(u8 addr,u8 reg,u8 len,u8 *data){
    u8 i;
    IOIIC_Start();
    IOIIC_Send_Byte(addr);
    if(IOIIC_Wait_Ack()){
        IOIIC_Stop();
        return -1;
    }
    IOIIC_Send_Byte(reg);
    IOIIC_Wait_Ack();
    for(i=0;i<len;i++){
        IOIIC_Send_Byte(data[i]);
        if(IOIIC_Wait_Ack()){
            IOIIC_Stop();
            return -1;
        }
    }
    IOIIC_Stop();
    return 0;
}

s8 I2C_READ_STRING(u8 addr,u8 reg,u8 len,u8 *buf){
    IOIIC_Start();
    IOIIC_Send_Byte(addr);
    if(IOIIC_Wait_Ack()){
        IOIIC_Stop();
        return -1;
    }
    IOIIC_Send_Byte(reg);
    IOIIC_Wait_Ack();
    IOIIC_Start();
    IOIIC_Send_Byte(addr+1);
    IOIIC_Wait_Ack();
    while(len){
        if(len==1)*buf=IOIIC_Read_Byte(0);
        else *buf=IOIIC_Read_Byte(1);
        buf++;
        len--;
    }
    IOIIC_Stop();
    return 0;
}

#define I2C_BUFFER_LEN 8
#define I2C0           5
#define BNO055_I2C_BUS_WRITE_ARRAY_INDEX ((u8)1)

s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    s32 BNO055_iERROR = BNO055_INIT_VALUE;
    u8 array[I2C_BUFFER_LEN];
    u8 stringpos = BNO055_INIT_VALUE;

    array[BNO055_INIT_VALUE] = reg_addr;
    for (stringpos = BNO055_INIT_VALUE; stringpos < cnt; stringpos++)
    {
        array[stringpos + BNO055_I2C_BUS_WRITE_ARRAY_INDEX] = *(reg_data + stringpos);
    }

    BNO055_iERROR = I2C_WRITE_STRING(dev_addr,reg_addr,cnt,array);
    return (s8)BNO055_iERROR;
}

s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt){
    s32 BNO055_iERROR = BNO055_INIT_VALUE;
    u8 array[I2C_BUFFER_LEN] = { BNO055_INIT_VALUE };
    u8 stringpos = BNO055_INIT_VALUE;

    array[BNO055_INIT_VALUE] = reg_addr;

    BNO055_iERROR = I2C_READ_STRING(dev_addr,reg_addr,cnt,reg_data);

    for (stringpos = BNO055_INIT_VALUE; stringpos < cnt; stringpos++)
    {
        *(reg_data + stringpos) = array[stringpos];
    }

    return (s8)BNO055_iERROR;
}

void BNO055_Init(GPIO_TypeDef* SCL_PORT,uint16_t SCL_PIN,GPIO_TypeDef* SDA_PORT,uint16_t SDA_PIN){
    struct bno055_t bno055;
    IOIIC_Init(SCL_PORT,SCL_PIN,SDA_PORT,SDA_PIN);
    bno055.bus_write = BNO055_I2C_bus_write;
    bno055.bus_read = BNO055_I2C_bus_read;
    bno055.delay_msec = delay_ms;
    bno055.dev_addr = BNO055_I2C_ADDR2;         //ARR low value
    bno055_init(&bno055);

    u8 gyro_calib_stat;
	BNO055_RETURN_FUNCTION_TYPE ret;
	
	bno055_set_sys_rst(BNO055_BIT_ENABLE);		
	delay_ms(1000);
	
	ret = bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
	
	while(1)
	{
		ret += bno055_get_gyro_calib_stat(&gyro_calib_stat);
		if(ret == BNO055_SUCCESS && gyro_calib_stat==3)
		{
			return;
		}
		delay_ms(500);
	}
}

s8 BNO055_READ_GYRO(struct bno055_gyro_t *gyro)
{
	BNO055_RETURN_FUNCTION_TYPE ret;
	ret = bno055_read_gyro_xyz(gyro);
	if(ret == BNO055_SUCCESS) return 1;
	return 0;
}
 
 
s8 BNO055_READ_ACC(struct bno055_accel_t *acc)
{
	BNO055_RETURN_FUNCTION_TYPE ret;
	ret = bno055_read_accel_xyz(acc);
	if(ret == BNO055_SUCCESS) return 1;
	return 0;
}
 
s8 BNO055_READ_MAG(struct bno055_mag_t *mag)
{
	BNO055_RETURN_FUNCTION_TYPE ret;
	ret = bno055_read_mag_xyz(mag);
	if(ret == BNO055_SUCCESS) return 1;
	return 0;
}
