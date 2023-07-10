#ifndef _MINE_BNO055_H_
#define _MINE_BNO055_H_

#include "system.h"
#include "bno055.h"

void BNO055_Init(GPIO_TypeDef* SCL_PORT,uint16_t SCL_PIN,GPIO_TypeDef* SDA_PORT,uint16_t SDA_PIN);
s8 BNO055_READ_GYRO(struct bno055_gyro_t *gyro);
s8 BNO055_READ_ACC(struct bno055_accel_t *acc);
s8 BNO055_READ_MAG(struct bno055_mag_t *mag);

#endif


