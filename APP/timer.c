#include "timer.h"

void TIM2_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2 , ENABLE);              //使能定时器2时钟
    TIM2_TimeBaseInit(TIM2_Prescaler_1 , TIM2_CounterMode_Up , 16000);    //设置定时器2为1分频，向上计数，计数值为16000即为1毫秒的计数值
    TIM2_ITConfig(TIM2_IT_Update , ENABLE);     //使能向上计数溢出中断
    TIM2_ARRPreloadConfig(ENABLE);  //使能定时器2自动重载功能
    TIM2_Cmd(ENABLE);               //启动定时器2开始计数
}

void TIM3_Init(void)
{
    TIM3_SetCounter(0); 
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3 , ENABLE);              //使能定时器3时钟
    TIM3_TimeBaseInit(TIM3_Prescaler_16 , TIM3_CounterMode_Up , 10000);    //设置定时器3为16分频，向上计数，计数值为10000即为10毫秒的计数值
    TIM3_ITConfig(TIM3_IT_Update , ENABLE);     //使能向上计数溢出中断
    TIM3_ARRPreloadConfig(ENABLE);  //使能定时器3自动重载功能
    TIM3_Cmd(ENABLE);               //启动定时器3开始计数
}