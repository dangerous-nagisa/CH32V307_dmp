#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H
#include "stdint.h"
#include "debug.h"


extern int SDA_bit;
extern int SCL_bit;

void SOFT_I2C_W_SCL(uint8_t BitValue);
 
void SOFT_I2C_W_SDA(uint8_t BitValue);
 
uint8_t SOFT_I2C_R_SDA(void);
 
void SOFT_I2C_Start(void);
 
void SOFT_I2C_Stop(void);
 
void SOFT_I2C_SendByte(uint8_t Byte);
 
uint8_t SOFT_I2C_ReceiveByte(void);
 
void SOFT_I2C_SendAck(uint8_t AckBit);
 
uint8_t SOFT_I2C_ReceiveAck(void);
 
void RCCdelay_us(uint32_t udelay);
#endif