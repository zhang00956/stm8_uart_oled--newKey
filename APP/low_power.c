#include "low_power.h"
#include <LCD_ZK.H>
#include "uart.h"
#include "sysclk.h"
#include "key.h"
#include "mini-printf.h"

#if USE_LPOWER

void GPIO_LowPower_Config(void)
{
// Port ABCDEFG in output push-pull 0
//    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);

//    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //����   0:1 ��
//    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);

    USART_DeInit(USART1);
    USART_Cmd(USART1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, DISABLE); //ʹ�ܴ���ʱ��


    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_IT);//���ڻ���
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);
    EXTI_SetPinSensitivity (EXTI_Pin_2, EXTI_Trigger_Falling); //�ⲿ�ж�1���½��ش���
    EXTI_ClearITPendingBit(EXTI_IT_Pin2);


    GPIO_Init(KEYPORT, KEYPIN, GPIO_Mode_In_PU_IT);//��ʼ��������GPB6��������
    EXTI_SetPinSensitivity (KEY_EXTI_PIN, EXTI_Trigger_Falling); //�ⲿ�ж�1���½��ش���
    EXTI_ClearITPendingBit(EXTI_IT_Pin6);

    GPIO_Init(ADCPORT, ADCPIN, GPIO_Mode_In_FL_IT);//��ʼ���ӿ�
    EXTI_SetPinSensitivity (ADC_EXTI_PIN, EXTI_Trigger_Rising); //����ж���������
    EXTI_ClearITPendingBit(EXTI_IT_Pin1);

//    EXTI_SetPinSensitivity(ADC_EXTI_PIN, EXTI_Trigger_Rising); //�������ж�
//    GPIO_Init(ADCPORT, ADCPIN, GPIO_Mode_In_FL_IT);//��ʼ���ӿ�

    /* Select LSI as system clock source */
    CLK_LSICmd(ENABLE);
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���л�
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//ѡ���ⲿ����ʱ����Ϊʱ��Դ
    /* system clock prescaler: 1*/
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//����ϵͳʱ�ӷ�Ƶ
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI);//�ȴ�ʱ���ȶ�

}
void EnterHaltSleep(void)
{
    /* Set GPIO*/
    disableInterrupts();
    GPIO_LowPower_Config();
    enableInterrupts();
    halt();
}

void ExitHaltSleep(void)
{
    extern u8 KeyWake;
    u8 ack = 0xf1;
    CLK_DeInit( );//��λ����ʱ��״̬
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���л�
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//ѡ���ⲿ����ʱ����Ϊʱ��Դ
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);

    MyUart_Init();
    if(KeyWake) {
        KeyWake = 0;
        uart_txarr(&ack, 1, 1); //�ظ�F1
    }
//    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);//key
//    initial_lcd();
    /*******/
}


#endif

void ADCConver_Init(void)
{
    u8 idx;

    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1 , ENABLE);              //ʹ��ADC1ʱ��
    GPIO_Init(ADCPORT , ADCPIN , GPIO_Mode_In_FL_No_IT);  //����ADCPIN�������룬���жϽ�ֹ

    ADC_Cmd(ADC1 , ENABLE);               //ʹ��ADC
    for (idx = 0; idx < 50; idx++); //adc�ϵ���Ҫһ��ʱ��

    ADC_VrefintCmd(ENABLE);//ʹ���ڲ��ο���ѹ
    ADC_Init(ADC1,
             ADC_ConversionMode_Single,   //����ת��ģʽ
             ADC_Resolution_12Bit,        //12λ����ת��е
             ADC_Prescaler_2              //ʱ������Ϊ2��Ƶ
            );

    ADC_ChannelCmd(ADC1,
                   ADC_Channel_17,         //����Ϊͨ��17���в���
                   ENABLE);
}


