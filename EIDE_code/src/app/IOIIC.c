#include "IOIIC.h"

static GPIO_TypeDef * LOC_SCL_PORT = 0;
static u16 LOC_SCL_Pin = 0;
static GPIO_TypeDef * LOC_SDA_PORT = 0;
static u16 LOC_SDA_Pin = 0;

void IOIIC_W_SCL(BitAction BitVal){
    GPIO_WriteBit(LOC_SCL_PORT, LOC_SCL_Pin, BitVal);
}

void IOIIC_W_SDA(BitAction BitVal){
    GPIO_WriteBit(LOC_SDA_PORT, LOC_SDA_Pin, BitVal);
}

BitAction IOIIC_R_SDA(GPIO_TypeDef * SDA_PORT, u16 SDA_Pin){
    return (BitAction)(GPIO_ReadInputDataBit(SDA_PORT, SDA_Pin));
}

void IOIIC_Init(GPIO_TypeDef * SCL_PORT, u16 SCL_Pin, GPIO_TypeDef * SDA_PORT, u16 SDA_Pin){
    LOC_SCL_PORT = SCL_PORT;LOC_SCL_Pin = SCL_Pin;
    LOC_SDA_PORT = SDA_PORT;LOC_SDA_Pin = SDA_Pin;
    //exactlly define SCL and SDA will cause a condition that we could only use one IOIIC at the same time without changing the codes.

    GPIO_RCC_ENABLE(SCL_PORT);
    GPIO_RCC_ENABLE(SDA_PORT);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SCL_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCL_PORT,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SDA_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SDA_PORT,&GPIO_InitStructure);

    IOIIC_W_SCL(Bit_SET);
    IOIIC_W_SDA(Bit_SET);
}

void IOIIC_Start(void){
    IOIIC_W_SDA(Bit_SET);
    IOIIC_W_SCL(Bit_SET);
    IOIIC_W_SDA(Bit_RESET);
    IOIIC_W_SCL(Bit_RESET);
}

void IOIIC_Stop(void){
    IOIIC_W_SDA(Bit_RESET);
    IOIIC_W_SCL(Bit_SET);
    IOIIC_W_SDA(Bit_SET);
}

void IOIIC_Ack(void){
    IOIIC_W_SCL( Bit_RESET);
    IOIIC_W_SDA( Bit_RESET);
    IOIIC_W_SCL( Bit_SET);
    IOIIC_W_SCL( Bit_RESET);
}

void IOIIC_NAck(void){
    IOIIC_W_SCL( Bit_RESET);
    IOIIC_W_SDA( Bit_SET);
    IOIIC_W_SCL( Bit_SET);
    IOIIC_W_SCL( Bit_RESET);
}

void IOIIC_W_Byte(uint8_t Byte){
    uint8_t i;
    for(i = 0; i < 8; i++){
        IOIIC_W_SDA((BitAction)(Byte & (0x80 >> i)));
        IOIIC_W_SCL( Bit_SET);
        IOIIC_W_SCL( Bit_RESET);
    }
    IOIIC_W_SCL( Bit_SET);
    IOIIC_W_SCL( Bit_RESET);
}
