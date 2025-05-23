#include "mpu6050.h"
#include "Soft_I2C.h"
#include "mpu6050_regs.h"
#include "string.h"
#include "stdio.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050_DMP.h"
#define MPU6050_ADDRESS		0xD0 // ����MPU6050�ĵ�ַ
 
uint8_t i = 10;
float pitch, roll, yaw;    // ŷ����
short aacx, aacy, aacz;    // ���ٶȴ�����ԭʼ����
short gyrox, gyroy, gyroz; // ������ԭʼ����
unsigned long walk;
float steplength = 0.3, Distance; // ����/��
uint8_t svm_set = 1;              // ·��
short GX, GY, GZ;


int8_t MPU6050_WriteReg_HAL(
    unsigned char slave_addr,   // �豸��ַ��7λ������0x68��
    unsigned char reg_addr,     // �Ĵ�����ַ��8λ��
    unsigned char length,       // ���ݳ���
    unsigned char const *data   // ��д������ݻ�����
) {
    SOFT_I2C_Start();
 
    // �����豸��ַ������1λ + дģʽ��
    SOFT_I2C_SendByte((slave_addr << 1) | 0x00); // �ؼ��޸ĵ�
    if (SOFT_I2C_ReceiveAck() != 0) { // ACK=0��ʾ�ɹ�
        SOFT_I2C_Stop();
        return -1; // �豸����Ӧ
    }
 
    // ���ͼĴ�����ַ
    SOFT_I2C_SendByte(reg_addr);
    if (SOFT_I2C_ReceiveAck() != 0) {
        SOFT_I2C_Stop();
        return -2; // �Ĵ�����ַACK����
    }
 
    // ��������
    for (unsigned char i = 0; i < length; i++) {
        SOFT_I2C_SendByte(data[i]);
        if (SOFT_I2C_ReceiveAck() != 0) {
            SOFT_I2C_Stop();
            return -3; // ����ACK����
        }
    }
 
    SOFT_I2C_Stop();
    return 0; // �ɹ�
}
int8_t MPU6050_ReadReg_HAL(
    unsigned char slave_addr,   // �豸��ַ��7λ��
    unsigned char reg_addr,     // �Ĵ�����ַ��8λ��
    unsigned char length,       // ���ݳ���
    unsigned char *data         // ���ݽ��ջ�����
) {
    SOFT_I2C_Start();
 
    // �����豸��ַ������1λ + дģʽ��
    SOFT_I2C_SendByte((slave_addr << 1) | 0x00); // �ؼ��޸ĵ�
    if (SOFT_I2C_ReceiveAck() != 0) { // ACK=0��ʾ�ɹ�
        SOFT_I2C_Stop();
        return -1; // �豸����Ӧ
    }
 
    // ���ͼĴ�����ַ
    SOFT_I2C_SendByte(reg_addr);
    if (SOFT_I2C_ReceiveAck() != 0) {
        SOFT_I2C_Stop();
        return -2; // �Ĵ�����ַACK����
    }
 
    // ���·�����ʼ�������л�Ϊ��ģʽ
    SOFT_I2C_Start();
    SOFT_I2C_SendByte((slave_addr << 1) | 0x01); // �ؼ��޸ĵ�
    if (SOFT_I2C_ReceiveAck() != 0) {
        SOFT_I2C_Stop();
        return -1; // �豸����Ӧ
    }
 
    // ��������
    for (unsigned char i = 0; i < length; i++) {
        uint8_t ack = (i == length - 1) ? 1 : 0; // ���һ���ֽڷ���NACK
        data[i] = SOFT_I2C_ReceiveByte();
        SOFT_I2C_SendAck(ack); // ����ACK/NACK
    }
 
    SOFT_I2C_Stop();
    return 0; // �ɹ�
}
// ��MPU6050�Ĵ���д������
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	SOFT_I2C_Start(); // ��ʼI2Cͨ��
	SOFT_I2C_SendByte(MPU6050_ADDRESS); // �����豸��ַ
	SOFT_I2C_ReceiveAck(); // �ȴ�Ӧ��
	SOFT_I2C_SendByte(RegAddress); // ���ͼĴ�����ַ
	SOFT_I2C_ReceiveAck(); // �ȴ�Ӧ��
	SOFT_I2C_SendByte(Data); // ��������
	SOFT_I2C_ReceiveAck(); // �ȴ�Ӧ��
	SOFT_I2C_Stop(); // ֹͣI2Cͨ��
}
 
// ��MPU6050�Ĵ�����ȡ����
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	SOFT_I2C_Start(); // ��ʼI2Cͨ��
	SOFT_I2C_SendByte(MPU6050_ADDRESS); // �����豸��ַ
	SOFT_I2C_ReceiveAck(); // �ȴ�Ӧ��
	SOFT_I2C_SendByte(RegAddress); // ���ͼĴ�����ַ
	SOFT_I2C_ReceiveAck(); // �ȴ�Ӧ��
	
	SOFT_I2C_Start(); // ���¿�ʼI2Cͨ��
	SOFT_I2C_SendByte(MPU6050_ADDRESS | 0x01); // �����豸��ַ�������ö�����λ
	SOFT_I2C_ReceiveAck(); // �ȴ�Ӧ��
	Data = SOFT_I2C_ReceiveByte(); // ��������
	SOFT_I2C_SendAck(1); // ����Ӧ���ź�
	SOFT_I2C_Stop(); // ֹͣI2Cͨ��
	
	return Data; // ���ض�ȡ��������
}
 
// ��ʼ��MPU6050
void MPU6050_Init(void)
{
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // ���õ�Դ����Ĵ���
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); // ���õ�Դ����Ĵ���
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // ���ò����ʼĴ���
	MPU6050_WriteReg(MPU6050_CONFIG, 0x03); // �������üĴ���
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // �������������üĴ���
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // ���ü��ٶȼ����üĴ���
}
 
// ��ȡMPU6050���豸ID
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I); // ��ȡ�豸ID�Ĵ���
}
 
// ��ȡMPU6050�ļ��ٶȼƺ�����������
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H); // ��ȡ���ٶȼ�X����ֽ�
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); // ��ȡ���ٶȼ�X����ֽ�
	*AccX = (DataH << 8) | DataL; // �ϲ����ֽں͵��ֽڣ��õ����ٶȼ�X������
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H); // ��ȡ���ٶȼ�Y����ֽ�
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L); // ��ȡ���ٶȼ�Y����ֽ�
	*AccY = (DataH << 8) | DataL; // �ϲ����ֽں͵��ֽڣ��õ����ٶȼ�Y������
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H); // ��ȡ���ٶȼ�Z����ֽ�
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L); // ��ȡ���ٶȼ�Z����ֽ�
	*AccZ = (DataH << 8) | DataL; // �ϲ����ֽں͵��ֽڣ��õ����ٶȼ�Z������
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H); // ��ȡ������X����ֽ�
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L); // ��ȡ������X����ֽ�
	*GyroX = (DataH << 8) | DataL; // �ϲ����ֽں͵��ֽڣ��õ�������X������
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H); // ��ȡ������Y����ֽ�
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L); // ��ȡ������Y����ֽ�
	*GyroY = (DataH << 8) | DataL; // �ϲ����ֽں͵��ֽڣ��õ�������Y������
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H); // ��ȡ������Z����ֽ�
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L); // ��ȡ������Z����ֽ�
	*GyroZ = (DataH << 8) | DataL; // �ϲ����ֽں͵��ֽڣ��õ�������Z������
}

// ������ȡ����
void dmp_getwalk(void)
{
    dmp_get_pedometer_step_count(&walk);
    printf("����: %d\n", walk);  // ��ʾ������ֵ
    Distance = steplength * walk;
    printf("·��: %d cm\n", (int)(Distance * 10));  // ��ʾ·�̣���λΪ cm
}
 

 void mpu6050_task(void)
{
    int ret = 0;
    // ret = MPU6050_DMP_init();
    dmp_getwalk();
    if(ret == 0){
        for (int i = 0; i < 10; i++) {  // ���Զ�ȡ10��
            printf("\n--- ��ȡ���� #%d ---\n", i+1);
            
            ret = MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
            
            if (ret == 0) {
                printf("�ɹ�! ��̬��: Pitch=%d�� Roll=%d�� Yaw=%d��\r\n", 
                    (int)(pitch * 100), (int)(roll * 100), (int)(yaw * 100));
            } else {
                printf("��ȡʧ�ܣ�������: %d\r\n", ret);
            }
        }
    }
       
}
