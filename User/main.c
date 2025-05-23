/*ͷ�ļ�������*/
#include "debug.h"  //��Ҫϵͳͷ�ļ�
#include "timer.h"  //��ʱ��ͷ�ļ�
#include "MPU6050.h"
#include "mpu6050_DMP.h"
#include "inv_mpu.h"


/*����������*/
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//��ʱ��3�����ж�

/*����������*/
unsigned long int uwtick;             //ϵͳ��ʱ����

/* GPIO��ʼ�� */
void GPIO_Toggle_INIT(void)
{
    // ���iic
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ����PA4(SCL)��PA5(SDA)Ϊ��©���ģʽ�������ڲ�����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; 
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5);

}


/*ϵͳ��ʱ��ʱ��3�жϷ�����*/
void TIM3_IRQHandler(void)//ÿ1����
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
    {
      uwtick++;//ϵͳ��ʱ��������
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}


/*������������*/
typedef struct
{
    void (*task_func)(void);//������
    unsigned long int rate_ms;  //����ִ������
    unsigned long int last_run; //�����ϴ����е�ʱ��
}task_t;

task_t scheduler_task[]={
    {mpu6050_task,10,0},
        
};
unsigned char task_num;  //������������
void scheduler_init()
{
    task_num=sizeof(scheduler_task)/sizeof(task_t);  //������������
}
void scheduler_run()
{
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;  //���µ�ǰϵͳʱ��
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;//��¼��������ʱ��
            scheduler_task[i].task_func();//��������
        }
    }
}

void MPU6050_DMP_Debug(void) {
    float pitch, roll, yaw;
    int ret;
    
    printf("\n\n=== MPU6050 DMP���Գ���ʼ ===\n");
    
    // 1. ���Գ�ʼ��DMP
    ret = MPU6050_DMP_init();
    printf("DMP��ʼ������ֵ: %d (0��ʾ�ɹ�)\r\n", ret);
    
    if (ret != 0) {
        printf("DMP��ʼ��ʧ�ܣ�ֹͣ����\r\n");
        return;
    }
    
    // 2. ѭ����ȡ����
    for (int i = 0; i < 10; i++) {  // ���Զ�ȡ10��
        printf("\n--- ��ȡ���� #%d ---\n", i+1);
        
        ret = MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        
        if (ret == 0) {
            printf("�ɹ�! ��̬��: Pitch=%d�� Roll=%d�� Yaw=%d��\r\n", 
                   (int)(pitch * 100), (int)(roll * 100), (int)(yaw * 100));
        } else {
            printf("��ȡʧ�ܣ�������: %d\r\n", ret);
        }
        
        Delay_Ms(100);  // �ȴ�100ms
    }
    
    printf("=== MPU6050 DMP���Գ������ ===\n\n");
}

/*������*/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
    GPIO_Toggle_INIT();

	printf("SystemClk:%d\r\n",SystemCoreClock);

    Tim3_Init(1000,96-1);//ϵͳ��ʱ��ʱ����ʼ��
    scheduler_init();

    MPU6050_DMP_init();
    u8 ID = MPU6050_GetID();                
    printf("ID:%x\r\n", ID);


	while(1)
    {
	    scheduler_run();//����������

        // float roll,pitch,yaw;
        // int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;

        // MPU6050_DMP_Debug();

    //     // // ��ȡԭʼ���ٶȺ�����������
    //     MPU6050_GetData(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
    //     // printf("Acc: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d\r\n", 
    //     // accX, accY, accZ, gyroX, gyroY, gyroZ);
        
    //    int ret = MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        
    //     if (ret == 0) {
    //         printf("�ɹ�! ��̬��: Pitch=%d�� Roll=%d�� Yaw=%df��\r\n", 
    //               pitch * 100, roll * 100, yaw * 100);
    //     } else {
    //         printf("��ȡʧ�ܣ�������: %d\r\n", ret);
    //     }
        
        // Delay_Ms(1000);
	}
}

