#include "beep.h"
#include "iostm8l152k4.h"
#define BEEP_PORT PA_ODR_bit.ODR6
void beep_delay(unsigned int i)
{
	while(i--);
}
void sound1()  //�Ȼ�������
{
	unsigned int i=5000;
	while(i--)			   //����һ��ʱ���PWM����ʹ����������
	{
		BEEP_PORT = 0;               //GPIO_SetBits(GPIOA,GPIO_Pin_6);		 //IO������ߵ�ƽ
		beep_delay(i);
		BEEP_PORT = 1;              //GPIO_ResetBits(GPIOA,GPIO_Pin_6);	  //IO������͵�ƽ
		beep_delay(i--);
	}	
}

/*******************************************************************************
* �� �� ��         : sound2
* ��������		   : ��������������	  ͨ���ı�Ƶ�ʿ��������仯
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void sound2()  //�綯������
{
	unsigned int i=800;
	while(i--)					//����һ��ʱ���PWM����ʹ����������
	{
		BEEP_PORT = 0;          //GPIO_SetBits(GPIOA,GPIO_Pin_6);	   //IO������ߵ�ƽ
		beep_delay(i);
		BEEP_PORT = 1;          //GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//IO������͵�ƽ
		beep_delay(i--);
	}	
}
