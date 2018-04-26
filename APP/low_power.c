#include "low_power.h"
#include <LCD_ZK.H>
#include "uart.h"
#include "sysclk.h"




void GPIO_LowPower_Config(void)
{
// Port ABCDEFG in output push-pull 0
//    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);
  
    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //辅助   0:1 灭
    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);   
    

    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_IT);//串口唤醒
    EXTI_SetPinSensitivity (EXTI_Pin_2, EXTI_Trigger_Falling); //外部中断1，下降沿触发

    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_PU_IT);//初始化按键，GPB6上拉输入
    EXTI_SetPinSensitivity (EXTI_Pin_6, EXTI_Trigger_Falling); //外部中断1，下降沿触发    
    
//    GPIO_Init(ADCPORT, ADCPIN, GPIO_Mode_In_FL_IT);//初始充电接口
//    EXTI_SetPinSensitivity (EXTI_Pin_6, EXTI_Trigger_Falling); //外部中断1，下降沿触发    
 
    
    /* Select LSI as system clock source */
    CLK_LSICmd(ENABLE);
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//选择外部低速时钟作为时钟源
    /* system clock prescaler: 1*/
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//设置系统时钟分频
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI);//等待时钟稳定
    



}
void EnterHaltSleep(void)
{
    /* Set GPIO*/
    GPIO_LowPower_Config();

    enableInterrupts();
    halt();
}

void ExitHaltSleep(void)
{
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//选择外部低速时钟作为时钟源
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //内部时钟为1分频 = 16Mhz
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);
  
    MyUart_Init();
    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);//key
//    initial_lcd();
    /*******/    
}

void ADCConver_Init(void)
{
    u8 idx;

    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1 , ENABLE);              //使能ADC1时钟
    GPIO_Init(ADCPORT , ADCPIN , GPIO_Mode_In_FL_No_IT);  //设置ADCPIN悬空输入，并中断禁止

    ADC_Cmd(ADC1 , ENABLE);               //使能ADC
    for (idx = 0; idx < 50; idx++); //adc上电需要一段时间

    ADC_VrefintCmd(ENABLE);//使能内部参考电压
    ADC_Init(ADC1,
             ADC_ConversionMode_Single,   //单次转换模式
             ADC_Resolution_12Bit,        //12位精度转换械
             ADC_Prescaler_2              //时钟设置为2分频
            );

    ADC_ChannelCmd(ADC1,
                   ADC_Channel_17,         //设置为通道17进行采样
                   ENABLE);
}


