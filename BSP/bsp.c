#include "bsp.h"

 

void BSP_Init(void)
{
 
 LED_Init( );//��ʼ��LED
 //LCD_GLASS_Init( );//lcd��ʼ��
 
 RTC_Periph_Init( );
 KEY_Init( );
 ADC_Periph_Init( );
 MyUart_Init( );
 
 enableInterrupts();//ʹ���ж�  
}