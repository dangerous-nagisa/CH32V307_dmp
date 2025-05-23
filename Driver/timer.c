#include "debug.h"

void Tim3_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitStruct.TIM_ClockDivision=0;//时钟分割
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//设置为向上计数
    TIM_TimeBaseInitStruct.TIM_Period=arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc;

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_Trigger, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, ENABLE);
}