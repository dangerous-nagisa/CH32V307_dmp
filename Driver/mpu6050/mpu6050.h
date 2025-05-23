#ifndef _MPU6050_H
#define _MPU6050_H
#include "stdint.h"
#include "debug.h"
 
int8_t MPU6050_WriteReg_HAL(
    unsigned char slave_addr,   // �豸��ַ��7λ������0x68��
    unsigned char reg_addr,     // �Ĵ�����ַ��8λ��
    unsigned char length,        // ���ݳ���
    unsigned char const *data    // ��д������ݻ�����
);
int8_t MPU6050_ReadReg_HAL(
    unsigned char slave_addr,   // �豸��ַ��7λ��
    unsigned char reg_addr,     // �Ĵ�����ַ��8λ��
    unsigned char length,       // ���ݳ���
    unsigned char *data         // ���ݽ��ջ�����
);// ��MPU6050�Ĵ���д�����ݵĺ���
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
 
// ��MPU6050�Ĵ�����ȡ���ݵĺ���
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
 
// ��ʼ��MPU6050�ĺ���
void MPU6050_Init(void);
 
// ��ȡMPU6050���豸ID�ĺ���
uint8_t MPU6050_GetID(void);
 
// ��ȡMPU6050�ļ��ٶȼƺ����������ݵĺ���
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);


void mpu6050_task(void);

#endif