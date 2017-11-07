#include "adc.h"
#include "stm8l15x.h"
#include "stm8l15x_dma.h"

#define DMA_BUF_SIZE 4

//����λ��
volatile  unsigned short  ADC_ConvertedValue[DMA_BUF_SIZE];
volatile float ADC1_Channel1_Battery,ADC1_Channel_Vrefint;
#define ADC1_DR_Address    ((uint16_t)(0x5344) )//ADC1�����ַ
  
void ADC_Periph_Init(void)
{
  u8 idx;
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);//ʹ��ADCʱ�� 
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);//ʹ��DMA1ʱ��
  
  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_In_FL_No_IT);//��������
  
  ADC_Cmd(ADC1,ENABLE);//��ADC
   
  for (idx = 0;idx < 50;idx++);//adc�ϵ���Ҫһ��ʱ��
       
  ADC_VrefintCmd(ENABLE);//ʹ���ڲ��ο���ѹ
  
  ADC_DMACmd(ADC1,ENABLE);//ʹ��ADC��DMA����
  
  //����ת��ģʽ
  ADC_Init(ADC1, ADC_ConversionMode_Continuous,ADC_Resolution_12Bit, ADC_Prescaler_1);
  
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels,ADC_SamplingTime_192Cycles);
                           
                            
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);//�ڲ��ο���ѹ
  ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);//ͨ��1   PA5
  
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);
 
  DMA_Init(DMA1_Channel0,//ͨ��0
          (uint32_t)&ADC_ConvertedValue,//��������ַ
          (uint16_t)ADC1_DR_Address,//ADC���ݼĴ�����ַ
          DMA_BUF_SIZE,//��������С
          DMA_DIR_PeripheralToMemory,//���赽�ڴ�
          DMA_Mode_Circular,//ѭ��ת��
          DMA_MemoryIncMode_Inc,//�ڴ��ַ����
          DMA_Priority_High,//���ȼ�Ϊ��
          DMA_MemoryDataSize_HalfWord);//�ڴ������ǰ���λ��
   
  DMA_SetCurrDataCounter(DMA1_Channel0,DMA_BUF_SIZE);//����ת�ٵ����ݸ��� 
 
  DMA_ITConfig(DMA1_Channel0,DMA_ITx_TC, ENABLE);//ʹ�ܴ�������ж�
  DMA_ITConfig(DMA1_Channel0,DMA_ITx_HT, DISABLE);
  
  DMA_Cmd(DMA1_Channel0,ENABLE);//ʹ��ͨ��0    
  
  DMA_GlobalCmd(ENABLE);//��DMA
  
  ADC_SoftwareStartConv(ADC1);//software trigger
}
