#include "stm8l15x.h"
#include "sysclk.h"

void SYSCLK_Init()
{
  CLK_DeInit( );//��λ����ʱ��״̬
  
  CLK_HSICmd(ENABLE);//ʹ��HSI ��ʵ��λ֮���Զ�������ʹ��HSI
  
  CLK_LSICmd(DISABLE);//�ر�LSI   38khz
  
  CLK_LSEConfig(CLK_LSE_ON);//��LSE  ʹ���ⲿ����
  
  CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���Զ��л�
  
  CLK_HSEConfig(CLK_HSE_ON);//��HSE ʹ���ⲿ����
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);//ѡ��HSE��ΪSYSCLK
  
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE);
  
//  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//��ʱ
   
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//16M��HSE 1��Ƶ��SYSCLKΪ16MHZ
  
  //��ʱ���л��ɹ�֮��ر�HSI
  CLK_HSICmd(DISABLE);

  CLK_MainRegulatorCmd(ENABLE);// ʹ������ѹ������
  
  CLK_ClockSecuritySystemEnable( );//ʹ��ʱ�Ӱ�ȫ���ƣ���HSE�������ʱ���Զ��л���HSI
  
  CLK_ITConfig(CLK_IT_SWIF,ENABLE);//ʱ���л��жϿ��������ڹ۲��л��¼� 
  
  CLK_HaltConfig(CLK_Halt_FastWakeup, ENABLE);//���ٻ���,�˴�����ʹ�ܣ���Ϊ������ʹ����֮��ʹ��HSIΪ��ʱ��
  
  CLK_HaltConfig(CLK_Halt_SlowWakeup, DISABLE);//��active-haltģʽ��MVR���ڴ�״̬
  
  while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF) != SET);//ֻ�е��ڲ��ο���ѹ�ȶ���ʱ��ſ����޸����µ�ֵ
   
  //Ϊ�˼��ٻ���ʱ�䣬ʹ�ܿ��ٻ���֮����������Ҫ�ȴ��ڲ��ο���ѹ������ʱ��
  PWR_FastWakeUpCmd(ENABLE); 
  
  //ʹ�ܳ����͵�ѹ�Ĺ��ܣ���HALTģʽ���Զ��ر��ڲ��ο���ѹ
  //���ѵ�ʱ���Զ�������ȱ���ǻ����¼��Ƚϳ�������ǰ��Ŀ�������ģʽ�����ֲ�
  PWR_UltraLowPowerCmd(ENABLE);
  
  PWR_PVDCmd(ENABLE);//ʹ�ܵ�ѹ�����
  
  PWR_PVDITConfig(ENABLE);//ʹ�ܵ�ѹ������ж�
  
  PWR_PVDLevelConfig(PWR_PVDLevel_1V85);//1.85V

}
   
//---  ΢�뼶��ʱ--------HSE-16M------------------   
void delayus(void)   
{    
    asm("nop"); //һ��asm("nop")��������ʾ�������Դ���100ns   
    asm("nop");   
    asm("nop");   
    asm("nop");    
}   
  
//---- ���뼶��ʱ����-------HSE-16M----------------   
void delayms(unsigned int time)   
{   
    unsigned int i;   
    while(time--)     
    for(i=900;i>0;i--)   
    delayus();    
}  


