//////////////////////////////////////////////////////////////////////////////////	 
//  ��������   : �ֿ�� OLED SPI�ӿ���ʾ����(STM8Lϵ��)
//              ˵��: 
//              ------------------------����ΪOLED��ʾ���õ��Ľ�?---------------------------------------
//              GND    ��Դ��
//              VCC   ��5V��3.3v��Դ
//              CLK   PB5��CLK��
//              MOSI  PB6��DIN��
//              DC    PB2 ��data or cmd��
//              CS1   PB3
//              FSO   PB7  (MISO)              
//              CS2   PB4
//
//******************************************************************************/
#include <LCD_ZK.H>
#include "stm8l15x.h"

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long



unsigned char const bmp1[]={
/*--  ������һ��ͼ��D:\�ҵ��ĵ�\My Pictures\12864-555.bmp  --*/
/*--  ����x�߶�=128x64  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xE0,0x60,0xE0,0xE0,0xE0,0xE0,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0x80,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x18,0x1C,0x0C,
0x0E,0x07,0x03,0x03,0x01,0x81,0xE0,0x78,0x1C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x1C,0x79,0xE1,0x83,
0x03,0x07,0x0E,0x0C,0x1C,0x18,0x38,0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,
0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0xF0,0x78,0x1E,0x07,0x03,0x81,0x80,0xC0,0xC0,0xC0,0xE0,0x60,
0x60,0x60,0x70,0xF0,0xFE,0x3F,0x19,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x38,0x31,0x3F,
0xFE,0xF0,0x60,0x60,0xE0,0xC0,0xC0,0xC0,0x80,0x81,0x03,0x07,0x1E,0x78,0xF0,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFC,0xFF,0x7F,0x07,0x0F,0x1F,0x3D,
0x79,0x71,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
0x80,0xC0,0xE0,0xF0,0x71,0x79,0x3D,0x1F,0x0F,0xFF,0xFE,0xFC,0x00,0x00,0x00,0x00,
0x00,0x80,0xFE,0xFF,0xF9,0x1C,0x0E,0x07,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x0E,0x1C,0xF9,0xFF,
0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x0E,0x9E,0xFC,0xF8,0xF0,0xE0,0xC0,0xC0,0x80,
0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xE0,0xF0,0xF8,0xF8,0xFC,0x9E,0x0F,0x07,
0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0xFF,0xDF,0x38,0x70,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0x70,0x38,0xDF,0xFF,
0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x80,0xC0,0xE0,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x03,0x01,0x00,0x01,0x01,0x03,0x07,
0x0F,0x1E,0x1E,0x0E,0x0F,0x07,0x03,0x01,0x01,0x00,0x01,0x01,0x03,0x07,0x0F,0x1E,
0x3C,0x78,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x07,0x0F,0x3C,0x70,0xE0,0xC1,0x81,0x03,0x03,0x03,0x07,0x06,
0x06,0x06,0x0E,0x0F,0x7F,0xFC,0x98,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x0C,0x8C,0xFC,
0x7F,0x0F,0x06,0x06,0x07,0x03,0x03,0x03,0x81,0xC1,0xE0,0x70,0x3C,0x0F,0x07,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0x3F,0x7C,0xFC,0xFE,0xEF,
0xE7,0xE3,0xE1,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE1,0xE3,0xE7,0xEF,0xFE,0x7C,0x3F,0x1F,0x0F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x0C,0x1C,0x18,
0x38,0x70,0x60,0xE0,0xC0,0xC1,0x87,0x9E,0xB8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xB8,0xDE,0xC7,0xE1,
0x60,0x70,0x38,0x18,0x1C,0x0C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x07,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x01,0x01,0x01,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};


unsigned char const jiong1[]={/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00,
0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00};

unsigned char const lei1[]={/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00,
0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00};

void OLED_GPIO_Config(void);
void GBZK_GPIO_Config(void);







//void GBZK_GPIO_Config(void)//demo��
//{		
//
//	GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
//    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
//}
void GBZK_GPIO_Config(void)//��ͷ
{		

	GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
}

/*дָ�LCDģ��*/
void transfer_command_lcd(int data1)   
{
	char i;
	lcd_rs(0);;;
	lcd_cs1(0);
	for(i=0;i<8;i++)
   {lcd_sclk(0);;;
		
		if(data1&0x80) {lcd_sid(1);;;}
		else {lcd_sid(0);;;}
		lcd_sclk(1);
		asm("nop");;;
	//	lcd_sclk(0);;;
	 	data1<<=1;
   }
	 	lcd_rs(1);;;
	 lcd_cs1(1);
}

/*д���ݵ�LCDģ��*/
void transfer_data_lcd(int data1)
{
	char i;
	lcd_rs(1);;;
	lcd_cs1(0);
	for(i=0;i<8;i++)
   {
		lcd_sclk(0);;;
		if(data1&0x80) {lcd_sid(1);;;}
		else {lcd_sid(0);;;}
		lcd_sclk(1);;;
		asm("nop");;;
		//lcd_sclk(0);;;
	 	data1<<=1;
   }lcd_rs(1);;;
	 lcd_cs1(1);
}
//����OLED��ʾ    
void OLED_Display_On(void)
{
  transfer_command_lcd(0x8d);//OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
  transfer_command_lcd(0x14);//OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
  transfer_command_lcd(0xaf);//OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
  transfer_command_lcd(0x8d);//OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
  transfer_command_lcd(0x10);//OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
  transfer_command_lcd(0xae);//OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	
//���������ƶ�
void move_left(void)
{
    transfer_command_lcd(0x27);
    transfer_command_lcd(0x00);//a
    transfer_command_lcd(0x00);//��ʼ��ַ
    transfer_command_lcd(0x00);//����ʱ��
    transfer_command_lcd(0x07);//������ַ
    transfer_command_lcd(0x00);
    transfer_command_lcd(0xFF);
    transfer_command_lcd(0x2F);//��ʼ����
}
/*******************************************************************************
**�������ƣ�void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**���������������ʱ
**��ڲ�����unsigned int ms   ��������ʱ��ֵ
**�������
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  ͨ��һ������ѭ��������ʱ*/
    for(y = 1000 ; y > 0 ; y--);
}

/*LCDģ���ʼ��*/
void initial_lcd()
{delay(400);
	//OLED_GPIO_Config();
	GBZK_GPIO_Config();
	lcd_cs1(0);
	Rom_CS(1);
       
	transfer_command_lcd(0xAE);   //display off
	transfer_command_lcd(0x20);	//Set Memory Addressing Mode	
	transfer_command_lcd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	transfer_command_lcd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	transfer_command_lcd(0xc8);	//Set COM Output Scan Direction
	transfer_command_lcd(0x00);//---set low column address
	transfer_command_lcd(0x10);//---set high column address
	transfer_command_lcd(0x40);//--set start line address
	transfer_command_lcd(0x81);//--set contrast control register
	transfer_command_lcd(0xFF);
	transfer_command_lcd(0xa1);//--set segment re-map 0 to 127
	transfer_command_lcd(0xa6);//--set normal display
	transfer_command_lcd(0xa8);//--set multiplex ratio(1 to 64)
	transfer_command_lcd(0x3F);//
	transfer_command_lcd(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	transfer_command_lcd(0xd3);//-set display offset
	transfer_command_lcd(0x00);//-not offset
	transfer_command_lcd(0xd5);//--set display clock divide ratio/oscillator frequency
	transfer_command_lcd(0xf0);//--set divide ratio
	transfer_command_lcd(0xd9);//--set pre-charge period
	transfer_command_lcd(0x22); //
	transfer_command_lcd(0xda);//--set com pins hardware configuration
	transfer_command_lcd(0x12);
	transfer_command_lcd(0xdb);//--set vcomh
	transfer_command_lcd(0x20);//0x20,0.77xVcc
	transfer_command_lcd(0x8d);//--set DC-DC enable
	transfer_command_lcd(0x14);//
	transfer_command_lcd(0xaf);//--turn on oled panel 
	lcd_cs1(1);
}

void lcd_address(uchar page,uchar column)
{

	transfer_command_lcd(0xb0 + column);   /*����ҳ��ַ*/
	transfer_command_lcd(((page & 0xf0) >> 4) | 0x10);	/*�����е�ַ�ĸ�4λ*/
	transfer_command_lcd((page & 0x0f) | 0x00);	/*�����е�ַ�ĵ�4λ*/	
}

/*ȫ������*/
void clear_screen()
{
	unsigned char i,j;
	lcd_cs1(0);
	Rom_CS(1);	
	for(i=0;i<8;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x00);
		transfer_command_lcd(0x10);
		for(j=0;j<128;j++)
		{
		  	transfer_data_lcd(0x00);
		}
	}
 	lcd_cs1(1);
}

/*��ʾ128x64����ͼ��*/
void display_128x64(const uchar *dp)
{
	uint i,j;
	lcd_cs1(0);
	for(j=0;j<8;j++)
	{
		lcd_address(0,j);
		for (i=0;i<128;i++)
		{	
			transfer_data_lcd(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	lcd_cs1(1);
}


/*��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��*/
void display_graphic_16x16(uint page,uint column,const uchar *dp)
{
	uint i,j;
 	lcd_cs1(0);
	Rom_CS(1); 	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<16;i++)
		{	
			transfer_data_lcd(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
		page++;
	}
	lcd_cs1(1);
}


/*��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��*/
void display_graphic_8x16(uint page,uchar column,uchar *dp)
{
	uint i,j;
	lcd_cs1(0);	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<8;i++)
		{	
			transfer_data_lcd(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
		page++;
	}
	lcd_cs1(1);
}


/*��ʾ5*7����ͼ��ASCII, ��5x7����������ַ�������ͼ��*/
void display_graphic_5x7(uint page,uchar column,uchar *dp)
{
	uint col_cnt;
	uchar page_address;
	uchar column_address_L,column_address_H;
	page_address = 0xb0+page-1;
	
	lcd_cs1(0);	
	
	column_address_L =(column&0x0f)-1;
	column_address_H =((column>>4)&0x0f)+0x10;
	
	transfer_command_lcd(page_address); 		/*Set Page Address*/
	transfer_command_lcd(column_address_H);	/*Set MSB of column Address*/
	transfer_command_lcd(column_address_L);	/*Set LSB of column Address*/
	
	for (col_cnt=0;col_cnt<6;col_cnt++)
	{	
		transfer_data_lcd(*dp);
		dp++;
	}
	lcd_cs1(1);
}

/****��ָ�����Ѷ�ֿ�IC***/
void send_command_to_ROM( uchar datu )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		if(datu&0x80)
			{lcd_sid(1);;;asm("nop");}
		else
			{lcd_sid(0);;; asm("nop"); }
			datu = datu<<1;
			lcd_sclk(0);;;asm("nop");
			lcd_sclk(1);;;asm("nop");
	}
}

/****�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�***/
static uchar get_data_from_ROM( )
{
	
	uchar i;
	uchar ret_data=0;
	lcd_sclk(1);;;  asm("nop");
	//GBZK_ROMOUTRESET();
	for(i=0;i<8;i++)
	{
		//Rom_OUT(1);;; 
        asm("nop");
		lcd_sclk(0);;;asm("nop");
		ret_data=ret_data<<1;
		if( ROM_OUT )
			{ret_data=ret_data+1;;;asm("nop");}
		else
			{ret_data=ret_data+0;;;asm("nop");	}
		lcd_sclk(1);
	}
	//GBZK_ROMOUTRESET();
	return(ret_data);
}



/*����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ*/
/*������ȡ*/
void get_n_bytes_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pBuff,uchar DataLen )
{
	uchar i;
	Rom_CS(0);
	lcd_cs1(1);	
	lcd_sclk(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM(addrHigh);
	send_command_to_ROM(addrMid);
	send_command_to_ROM(addrLow);
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();
	Rom_CS(1);
}


/******************************************************************/

ulong  fontaddr=0;
void display_GB2312_string(uchar y,uchar x,uchar *text)
{
	uchar i= 0;
	uchar addrHigh,addrMid,addrLow ;
	uchar fontbuf[32];			
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			/*������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
            if((129 - x) >= 16){
                display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
            }else{
                x = 1;
                y += 2;
                display_graphic_16x16(y,x,fontbuf);
            }
			i+=2;
			x+=16;

		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			/*������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
            if((129 - x) >= 16){
                display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
            }else{
                x = 1;
                y += 2;
                display_graphic_16x16(y,x,fontbuf);
            }			
            i+=2;
			x+=16;

		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			unsigned char fontbuf[16];			
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			
			addrHigh = (fontaddr&0xff0000)>>16;
			addrMid = (fontaddr&0xff00)>>8;
			addrLow = fontaddr&0xff;

			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16 );/*ȡ16���ֽڵ����ݣ��浽"fontbuf[32]"*/
			if((129 - x) >= 8){
                display_graphic_8x16(y,x,fontbuf);/*��ʾ8x16��ASCII�ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			}else{
                x = 1;
                y += 2;
                display_graphic_8x16(y,x,fontbuf);
            }
            i+=1;
			x+=8;
		}
		else
			i++;	
	}
	
}

void display_string_5x7(uchar y,uchar x,uchar *text)
{
	unsigned char i= 0;
	unsigned char addrHigh,addrMid,addrLow ;
	while((text[i]>0x00))
	{
		if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			unsigned char fontbuf[8];			
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*8);
			fontaddr = (unsigned long)(fontaddr+0x3bfc0);			
			addrHigh = (fontaddr&0xff0000)>>16;
			addrMid = (fontaddr&0xff00)>>8;
			addrLow = fontaddr&0xff;

			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,8);/*ȡ8���ֽڵ����ݣ��浽"fontbuf[32]"*/
			
			display_graphic_5x7(y,x,fontbuf);/*��ʾ5x7��ASCII�ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			i+=1;
			x+=6;
		}
		else
		i++;	
	}
	
}

