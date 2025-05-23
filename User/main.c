/*头文件引用区*/
#include "debug.h"  //必要系统头文件
#include "timer.h"  //定时器头文件
#include "MPU6050.h"
#include "mpu6050_DMP.h"
#include "inv_mpu.h"


/*函数声明区*/
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//定时器3快速中断

/*变量声明区*/
unsigned long int uwtick;             //系统计时变量

/* GPIO初始化 */
void GPIO_Toggle_INIT(void)
{
    // 软件iic
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA4(SCL)和PA5(SDA)为开漏输出模式，启用内部上拉
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; 
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5);

}


/*系统计时定时器3中断服务函数*/
void TIM3_IRQHandler(void)//每1毫秒
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
    {
      uwtick++;//系统计时变量自增
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}


/*任务调度器相关*/
typedef struct
{
    void (*task_func)(void);//任务函数
    unsigned long int rate_ms;  //任务执行周期
    unsigned long int last_run; //任务上次运行的时间
}task_t;

task_t scheduler_task[]={
    {mpu6050_task,10,0},
        
};
unsigned char task_num;  //任务数量变量
void scheduler_init()
{
    task_num=sizeof(scheduler_task)/sizeof(task_t);  //计算任务数量
}
void scheduler_run()
{
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;  //更新当前系统时间
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;//记录本次运行时间
            scheduler_task[i].task_func();//运行任务
        }
    }
}

void MPU6050_DMP_Debug(void) {
    float pitch, roll, yaw;
    int ret;
    
    printf("\n\n=== MPU6050 DMP调试程序开始 ===\n");
    
    // 1. 尝试初始化DMP
    ret = MPU6050_DMP_init();
    printf("DMP初始化返回值: %d (0表示成功)\r\n", ret);
    
    if (ret != 0) {
        printf("DMP初始化失败，停止测试\r\n");
        return;
    }
    
    // 2. 循环读取数据
    for (int i = 0; i < 10; i++) {  // 尝试读取10次
        printf("\n--- 读取尝试 #%d ---\n", i+1);
        
        ret = MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        
        if (ret == 0) {
            printf("成功! 姿态角: Pitch=%d° Roll=%d° Yaw=%d°\r\n", 
                   (int)(pitch * 100), (int)(roll * 100), (int)(yaw * 100));
        } else {
            printf("读取失败，错误码: %d\r\n", ret);
        }
        
        Delay_Ms(100);  // 等待100ms
    }
    
    printf("=== MPU6050 DMP调试程序结束 ===\n\n");
}

/*主函数*/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
    GPIO_Toggle_INIT();

	printf("SystemClk:%d\r\n",SystemCoreClock);

    Tim3_Init(1000,96-1);//系统计时定时器初始化
    scheduler_init();

    MPU6050_DMP_init();
    u8 ID = MPU6050_GetID();                
    printf("ID:%x\r\n", ID);


	while(1)
    {
	    scheduler_run();//调度器运行

        // float roll,pitch,yaw;
        // int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;

        // MPU6050_DMP_Debug();

    //     // // 读取原始加速度和陀螺仪数据
    //     MPU6050_GetData(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
    //     // printf("Acc: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d\r\n", 
    //     // accX, accY, accZ, gyroX, gyroY, gyroZ);
        
    //    int ret = MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
        
    //     if (ret == 0) {
    //         printf("成功! 姿态角: Pitch=%d° Roll=%d° Yaw=%df°\r\n", 
    //               pitch * 100, roll * 100, yaw * 100);
    //     } else {
    //         printf("读取失败，错误码: %d\r\n", ret);
    //     }
        
        // Delay_Ms(1000);
	}
}

