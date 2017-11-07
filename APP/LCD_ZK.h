//////////////////////////////////////////////////////////////////////////////////	 
//  功能描述   : 字库版 OLED SPI接口演示例程(STM8L系列)
//              说明: 
//              ------------------------以下为OLED显示所用到的接?---------------------------------------
//              GND    电源地
//              VCC   接5V或3.3v电源
//              CLK   PB5（CLK）
//              MOSI  PB6（DIN）
//              DC    PB2 （data or cmd）
//              CS1   PB3
//              FSO   PB7  (MISO)              
//              CS2   PB4
//
//******************************************************************************/
#define _LCD_ZK_H_
#ifdef _LCD_ZK_H_



/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

//带参宏，可以像内联函数一样使用
#define lcd_cs1(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_3)

#define lcd_rs(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_2);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_2)


#define lcd_sid(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_1)

#define lcd_sclk(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define Rom_CS(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define Rom_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)


#define ROM_OUT    GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)

extern unsigned char const bmp1[];
extern unsigned char const jiong1[];
extern unsigned char const lei1[];


/*写指令到LCD模块*/
void transfer_command_lcd(int data1);   
/*写数据到LCD模块*/
void transfer_data_lcd(int data1);
/*延时*/
void delay(unsigned int ms);               
/*LCD模块初始化*/
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
/*全屏清屏*/
void clear_screen(void);
/*显示128x64点阵图像*/
void display_128x64(const unsigned char *dp);
/*显示132x64点阵图像*/
//void display_132x64(unsigned char *dp);
/*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/
void display_graphic_16x16(unsigned int page,unsigned int column,const unsigned char *dp);
/*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/
void display_graphic_8x16(unsigned int page,unsigned char column,unsigned char *dp);
/*显示5*7点阵图像、ASCII, 或5x7点阵的自造字符、其他图标*/
void display_graphic_5x7(unsigned int page,unsigned char column,unsigned char *dp);
/****送指令到晶联讯字库IC***/
void send_command_to_ROM( unsigned char datu );
/****从晶联讯字库IC中取汉字或字符数据（1个字节）***/
static unsigned char get_data_from_ROM(void);
/*从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
void get_n_bytes_data_from_ROM(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,unsigned char DataLen );
/******************************************************************/
void display_GB2312_string(unsigned char y,unsigned char x,unsigned char *text);
void display_string_5x7(unsigned char y,unsigned char x,unsigned char *text);
/*display on*/
void OLED_Display_On(void);
/*display off*/
void OLED_Display_Off(void);
/*move left*/
void move_left(void);
#endif
