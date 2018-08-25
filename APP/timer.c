#include "timer.h"

void TIM2_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2 , ENABLE);              //ʹ�ܶ�ʱ��2ʱ��
    TIM2_TimeBaseInit(TIM2_Prescaler_1 , TIM2_CounterMode_Up , 16000);    //���ö�ʱ��2Ϊ1��Ƶ�����ϼ���������ֵΪ16000��Ϊ1����ļ���ֵ
    TIM2_ITConfig(TIM2_IT_Update , ENABLE);     //ʹ�����ϼ�������ж�
    TIM2_ARRPreloadConfig(ENABLE);  //ʹ�ܶ�ʱ��2�Զ����ع���
    TIM2_Cmd(ENABLE);               //������ʱ��2��ʼ����
}

void TIM3_Init(void)
{
    TIM3_SetCounter(0); 
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3 , ENABLE);              //ʹ�ܶ�ʱ��3ʱ��
    TIM3_TimeBaseInit(TIM3_Prescaler_16 , TIM3_CounterMode_Up , 10000);    //���ö�ʱ��3Ϊ16��Ƶ�����ϼ���������ֵΪ10000��Ϊ10����ļ���ֵ
    TIM3_ITConfig(TIM3_IT_Update , ENABLE);     //ʹ�����ϼ�������ж�
    TIM3_ARRPreloadConfig(ENABLE);  //ʹ�ܶ�ʱ��3�Զ����ع���
    TIM3_Cmd(ENABLE);               //������ʱ��3��ʼ����
}