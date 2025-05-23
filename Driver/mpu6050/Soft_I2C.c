#include "Soft_I2C.h"
#include "debug.h"
// PA4(SCL)	PA5(SDA)


int SCL_bit = 1;
int SDA_bit = 1;



/*
*������I2CдSCL��ƽ
*������BitValue ��ǰ��Ҫд��SCL�ĵ�ƽ��ֵΪ0��1
*����ֵ����
*ע�������BitValueΪ0ʱ����SCLΪ�͵�ƽ����BitValueΪ1ʱ����SCLΪ�ߵ�ƽ
*/
void SOFT_I2C_W_SCL(uint8_t BitValue){
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (BitValue ? GPIO_PIN_SET : GPIO_PIN_RESET));
	if(BitValue) 
        GPIO_SetBits(GPIOA,GPIO_Pin_4);
    else 
     GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    SCL_bit = BitValue;
	RCCdelay_us(5);
}
 
/*
*������I2CдSDA��ƽ
*������BitValue ��ǰ��Ҫд��SDA�ĵ�ƽ��ֵΪ0��1
*����ֵ����
*ע�������BitValueΪ0ʱ����SDAΪ�͵�ƽ����BitValueΪ1ʱ����SCLΪ�ߵ�ƽ
*/
void SOFT_I2C_W_SDA(uint8_t BitValue){
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (BitValue ? GPIO_PIN_SET : GPIO_PIN_RESET));
	if(BitValue) 
        GPIO_SetBits(GPIOA,GPIO_Pin_5);
    else 
        GPIO_ResetBits(GPIOA,GPIO_Pin_5);
    SDA_bit = BitValue;
	RCCdelay_us(5);
}
 
/*
*������I2C��ȡSDA���ŵĵ�ƽ״̬
*��������
*����ֵ����ȡ���ĵ�ƽ
*ע������͵�ƽΪ0���ߵ�ƽΪ1
*/
uint8_t SOFT_I2C_R_SDA(void){
	uint8_t BitValue;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == Bit_RESET){
		BitValue = 0;
	}
	else{
		BitValue = 1;
	}
	return BitValue;
}
 
/*
*������I2C��ʼ�ź�
*��������
*����ֵ����
*ע�������
*/
void SOFT_I2C_Start(void){
	SOFT_I2C_W_SDA(1);
	SDA_bit = 1;
	SOFT_I2C_W_SCL(1);
	SCL_bit = 1;
	SOFT_I2C_W_SDA(0);
	SDA_bit = 0;
	SOFT_I2C_W_SCL(0);
	SCL_bit = 0;
}
 
/*
*������I2Cֹͣ�ź�
*��������
*����ֵ����
*ע�������
*/
void SOFT_I2C_Stop(void){
	SOFT_I2C_W_SDA(0);
	SDA_bit = 0;
	SOFT_I2C_W_SCL(1);
	SCL_bit = 1;
	SOFT_I2C_W_SDA(1);
	SDA_bit = 1;
}
 
/*
*������I2C����8λ����
*����������
*����ֵ����
*ע�������
*/
void SOFT_I2C_SendByte(uint8_t Byte){
	uint8_t i;
	for(i = 0; i < 8; i++){
		SOFT_I2C_W_SDA(Byte & (0x80 >> i));
		SDA_bit = (Byte & (0x80 >> i));
		SOFT_I2C_W_SCL(1);
		SCL_bit = 1;
		SOFT_I2C_W_SCL(0);
		SCL_bit = 0;
	}
}
/*
*������I2C����8λ����
*��������
*����ֵ������
*ע�������
*/
uint8_t SOFT_I2C_ReceiveByte(void){
	uint8_t Byte = 0x00;
	SOFT_I2C_W_SDA(1); //������sda��֮ǰ����һֱռ����sda���ͷ�sda���ôӻ�ռ�ݡ�
	SDA_bit = 1;
	uint8_t i;
	for(i = 0; i < 8; i++){
		SOFT_I2C_W_SCL(1); //������scl���ӻ�������ݷŵ�sda��
		SCL_bit = 1;
		if(SOFT_I2C_R_SDA() == 1){ //��0��ʱ���ø���ΪByte��00000000
			Byte |= (0x80 >> i);
		}
		SOFT_I2C_W_SCL(0); 
		SCL_bit = 0;
	}
	return Byte;
}
/*
*������I2C����Ӧ��λ
*������0��1
*����ֵ����
*ע�������
*/
void SOFT_I2C_SendAck(uint8_t AckBit){
	SOFT_I2C_W_SDA(AckBit);
	SDA_bit = AckBit;
	SOFT_I2C_W_SCL(1);
	SCL_bit = 1;
	SOFT_I2C_W_SCL(0);
	SCL_bit = 0;
}
/*
*������I2C����Ӧ��λ
*��������
*����ֵ��0��1
*ע�������
*/
uint8_t SOFT_I2C_ReceiveAck(void){
	 uint8_t AckBit;
    SOFT_I2C_W_SDA(1); // �ͷ�SDA����
    SDA_bit = 1;      // ȷ��������ʵ��״̬һ��
    SOFT_I2C_W_SCL(1); // ʱ������
    SCL_bit = 1;
    AckBit = SOFT_I2C_R_SDA(); // ��ȡACKλ
    SOFT_I2C_W_SCL(0); // ʱ������
    SCL_bit = 0;
    return AckBit;
}
 
/*
*������΢���ӳٺ���
*����������΢��
*����ֵ����
*ע�������
*/
void RCCdelay_us(uint32_t udelay){
	__IO uint32_t Delay = udelay * 72 / 8;
	
	do
	{
		__NOP();
	}
	while(Delay--);
}