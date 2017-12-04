//////////////////////////////////////////////////////////////////////////////////
//  功能描述   : 字库版 OLED SPI接口演示例程(STM8L系列)
//              说明:
//              ------------------------以下为OLED显示所用到的接?---------------------------------------
//              GND    电源地
//              VCC   接5V或3.3v电源
//              CLK   PB3（CLK）
//              MOSI  PD1（DIN）
//              DC    PD2 （data or cmd）
//              CS1   PD3
//              FSO   PD4  (MISO)
//              CS2   PD5
//相关命令定义在it.h里面
//******************************************************************************/

#include "stm8l15x.h"
#include "led.h"
#include "LCD_ZK.h"
#include "uart.h"
#include "stm8l15x_it.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "sysclk.h"
#include "beep.h"
#include "key.h"
#include "timer.h"

#define ADCPORT  GPIOB
#define ADCPIN   GPIO_Pin_1

#define OFF_TIME           500



queue Q, Q_old;
extern u8 num;//未读
char num_r = 0;//已读
char num_l = 0;//历史消息
//u16 key_num = 0; //长按计数
//u8 flag_next = 1;//继续往下执行
u8 led_on = 0;//第一次按按键点亮屏幕
u8 AppState = NORMAL;//默认正常状态
u8 LedF5 = 0;   //是否刷新显示收到消息
u8 beep = 0;//蜂鸣器命令
u8 keyPassValue = 0; //长按短按状态
char help[4] = {0xff, 0x0d, 0x0a, 0x00}; //求救信息
char CanHelp[4] = {0xfe, 0x0d, 0x0a, 0x00}; //取消报警
char AllRead[4] = {0xfd, 0x0d, 0x0a, 0x00}; //已读

void ADCConver_Init(void);
u16 ReadBattery(void);
int main(void)
{
    u8 arr[49];
    SNode pack, pack_temp;
    u8 buf[20]={0x00};//顶一个局部缓冲区
    u16 cnt_t = 0; //息屏计时
    u8 pos = 0;   //消息队列位置
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //内部时钟为1分频 = 16Mhz
    CreateQueue(&Q);//创建队列
    CreateQueue(&Q_old);
    MyUart_Init();
    //TIM3_Init();
    initial_lcd();
    clear_screen();    //clear all dots
    display_128x64(bmp1);
    /*******/
    ADCConver_Init();
    u16 u16_adc1_value;
    float adBattery = 0.0;
    
    uart_txstring("中文\r\n");
    uart_txHex(0xff);
    //uart_txarr(help,3,0);
    
    while(1){
       u16_adc1_value = ReadBattery();
       adBattery = 2.8 * u16_adc1_value / 4096;
       //sprintf(buf,"%.3f \r\n",adBattery);
       //uart_txstring(buf);
       delayms(1000);
    }
    /*****************/

    GPIO_Config();

//    transfer_command_lcd(0x27);
//    transfer_command_lcd(0x00);//a
//    transfer_command_lcd(0x00);//起始地址
//    transfer_command_lcd(0x00);//滚动时间
//    transfer_command_lcd(0x07);//结束地址
//    transfer_command_lcd(0x00);
//    transfer_command_lcd(0xFF);
//    transfer_command_lcd(0x2F);//开始滚动
    /*	lcd_cs1(0);
    	 transfer_command_lcd(0x2E);
        transfer_command_lcd(0x29);
        transfer_command_lcd(0x00);
        transfer_command_lcd(0x00);
        transfer_command_lcd(0x00);
        transfer_command_lcd(0x07);
        transfer_command_lcd(0x01);
        transfer_command_lcd(0x2F);
    	lcd_cs1(1);*/

    while(1) {
        switch(AppState) {
            case NORMAL:
                if((LedF5 == 1) && (led_on == 0)) {
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    sprintf((char *)buf, "收到%d条新消息！", num);
                    display_GB2312_string(0, 1, buf);
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        if(led_on) {             //息屏状态，单次按键，初次按下时点亮屏幕，不读消息
                            led_on = 0;
                            OLED_Display_On();
                            cnt_t = 0;     //重置屏幕熄灭时间
                        } else {                  //非单次按下，读消息
                            if(num > 0) {
                                num--;
                                num_r++;
                            } else {
                                num_r = 0;
                            }
                            if(DeQueue(&Q, &pack)) {
                                get_pack(&pack, arr);
                                if(!EnQueue(&Q_old, &pack)) {
                                    DeQueue(&Q_old, &pack_temp);
                                    EnQueue(&Q_old, &pack);
                                }
                                clear_screen();    //clear all dots
                                sprintf((char *)buf, "消息(%d)未读(%d)", num_r, num);
                                display_GB2312_string(0, 1, buf);
                                display_GB2312_string(2, 1, arr);
                                num_l = 0;
                                UART1_SendString(AllRead);   //暂定把已读功能放在这里
                            } else {
                                if (size_queue(&Q_old) > 0) {
                                    num_l++;
                                    if(num_l > size_queue(&Q_old)) {
                                        num_l = 1;
                                    }
                                    pos = (Q_old.front + num_l) % (size_queue(&Q_old) + 1);
                                    get_pack(&(Q_old.Pack[pos]), arr);
                                    clear_screen();    //clear all dots
                                    sprintf((char *)buf, "历史消息(%d)", num_l);
                                    display_GB2312_string(0, 1, buf);
                                    display_GB2312_string(2, 1, arr);
                                } else {
                                    clear_screen();    //clear all dots
                                    sprintf((char *)buf, "无消息!");
                                    display_GB2312_string(0, 1, buf);
                                }
                            }
                            cnt_t = 0;     //读了消息就重置屏幕熄灭时间
                        }
                        break;

                    case KeyPassLong :
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "求救成功!");
                        UART1_SendString(help);
                        beep = 1;
                        display_GB2312_string(0, 1, buf);
                        if(LedF5 == 1) {
                            sprintf((char *)buf, "收到%d条新消息！", num);
                            display_GB2312_string(2, 1, buf);
                            LedF5 = 0;
                        }
                        beep = 1;
                        cnt_t = 0;     //重置屏幕熄灭时间
                        AppState = CALLING;
                        break;

                    default :
                        break;
                }
                if(AppState == NORMAL) {
                    delayms(20);
                    cnt_t++;
                    if(cnt_t > OFF_TIME) {
                        cnt_t = 0;
                        OLED_Display_Off();
                        led_on = 1;
                    }
                }
                break;

            case WORNING:
                if(LedF5 == 1) {             //息屏状态，点亮屏幕
                    LedF5 = 0;
                    OLED_Display_On();
                    clear_screen();    //clear all dots
                    sprintf((char *)buf, "收到%d条新消息", num);
                    display_GB2312_string(0, 1, buf);
                    cnt_t = 0;     //重置屏幕熄灭时间
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "请长按解除警报!");
                        display_GB2312_string(0, 1, buf);
                        sprintf((char *)buf, "然后再读取消息!");
                        display_GB2312_string(2, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "警报已解除!");
                        display_GB2312_string(0, 1, buf);
                        UART1_SendString(CanHelp);
                        beep = 0;
                        BeepInit();
                        LEDInit();
                        TIM2_Cmd(DISABLE);
                        AppState = NORMAL;
                        break;
                    default:
                        break;
                }
                if(AppState == WORNING) {
                    if((beep > 0) && (KEY_NORMAL == KeyRead())) {  //有按键操作不再响铃
                        sound2();
                    }
                    delayms(20);
                }
                break;
            case CALLING:
                if(led_on) {             //息屏状态，点亮屏幕，并且不息屏
                    led_on = 0;
                    OLED_Display_On();
                }
                if(LedF5 == 1) {             //息屏状态，点亮屏幕
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    sprintf((char *)buf, "收到%d条新消息", num);
                    display_GB2312_string(0, 1, buf);
                    cnt_t = 0;     //重置屏幕熄灭时间
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "长按解除警报!");
                        display_GB2312_string(0, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "警报已解除!");
                        display_GB2312_string(0, 1, buf);
                        UART1_SendString(CanHelp);
                        BeepInit();
                        LEDInit();
                        TIM2_Cmd(DISABLE);
                        beep = 0;
                        AppState = NORMAL;
                        break;
                    default:
                        break;
                }
                if(AppState == CALLING) {
                    if((beep > 0) && (KEY_NORMAL == KeyRead())) {  //有按键操作不再响铃
                        sound2();
                    }
                    delayms(20);
                }
                break;
            default:
                break;
        }

    }
}

void ADCConver_Init(void)
{
    u8 idx;

    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1 , ENABLE);              //使能ADC1时钟
    GPIO_Init(ADCPORT , ADCPIN , GPIO_Mode_In_FL_No_IT);  //设置ADCPIN悬空输入，并中断禁止

    ADC_VrefintCmd(ENABLE);//使能内部参考电压
    ADC_Init(ADC1,
             ADC_ConversionMode_Single,   //单次转换模式
             ADC_Resolution_12Bit,        //12位精度转换械
             ADC_Prescaler_2              //时钟设置为2分频
            );

    ADC_ChannelCmd(ADC1,
                   ADC_Channel_17,         //设置为通道17进行采样
                   ENABLE);

    ADC_Cmd(ADC1 , ENABLE);               //使能ADC
    for (idx = 0; idx < 50; idx++); //adc上电需要一段时间
}

u16 ReadBattery(void)
{
    ADC_SoftwareStartConv(ADC1);      //启动ADC

    while(ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == 0);  //等待转换结束
    ADC_ClearFlag(ADC1 , ADC_FLAG_EOC);                 //清除中断标志
    return ADC_GetConversionValue(ADC1);           //读取ADC1，通道1的转换结果
}



