#include "beep.h"
#include "iostm8l152k4.h"
#define BEEP_PORT PA_ODR_bit.ODR6
void beep_delay(unsigned int i)
{
	while(i--);
}
void sound1()  //救护车报警
{
	unsigned int i=5000;
	while(i--)			   //产生一段时间的PWM波，使蜂鸣器发声
	{
		BEEP_PORT = 0;               //GPIO_SetBits(GPIOA,GPIO_Pin_6);		 //IO口输出高电平
		beep_delay(i);
		BEEP_PORT = 1;              //GPIO_ResetBits(GPIOA,GPIO_Pin_6);	  //IO口输出低电平
		beep_delay(i--);
	}	
}

/*******************************************************************************
* 函 数 名         : sound2
* 函数功能		   : 蜂鸣器报警函数	  通过改变频率控制声音变化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void sound2()  //电动车报警
{
	unsigned int i=800;
	while(i--)					//产生一段时间的PWM波，使蜂鸣器发声
	{
		BEEP_PORT = 0;          //GPIO_SetBits(GPIOA,GPIO_Pin_6);	   //IO口输出高电平
		beep_delay(i);
		BEEP_PORT = 1;          //GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//IO口输出低电平
		beep_delay(i--);
	}	
}
