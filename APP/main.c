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
//#include "stdio.h"
#include "string.h"
#include "sysclk.h"
#include "beep.h"
#include "key.h"
#include "timer.h"
#include "mini-printf.h"
#include "low_power.h"



#define LOW_POWER 2367
#define CHARGEING 3055
#define CHARGE_OVER 2773

#define OFF_TIME           1000



queue Q, Q_old;
extern u8 num;//未读
char num_r = 0;//已读
char num_l = 0;//历史消息
//u16 key_num = 0; //长按计数
//u8 flag_next = 1;//继续往下执行
volatile u8 led_on = 0;//第一次按按键点亮屏幕
volatile u8 AppState = NORMAL;//默认正常状态
volatile u8 LedF5 = 0;   //是否刷新显示收到消息
volatile u8 beep = 0;//蜂鸣器命令
volatile u8 keyPassValue = 0; //长按短按状态
volatile u8 Power_charge = 0; //充电器连接状态检测
volatile u16 screen_off_cnt = 0; //息屏计时
volatile u8 LowPower = 0;
extern volatile u16 TIM3_Conut;

/*0xff求救信息
 *0xfe取消报警
 *0xfd已读消息
 */
u8 HelpMsg[2] = {0xff, 0xfe};
u8 ReadMsg[3] = {0xfd,0x00,0x00};
u8 PowerMsg[4] = {0xfc, 0xfb, 0xfa, 0xf9};
void ADCConver_Init(void);
u16 ReadBattery(void);
void CheckPower(void);

int main(void)
{
    u8 power_on = 0;
    u8 arr[49]={0};
    SNode pack, pack_temp;
    u8 buf[20] = {0x00}; //顶一个局部缓冲区    
    u8 pos = 0;   //消息队列位置
    u16 callID = 0;
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//选择外部低速时钟作为时钟源
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //内部时钟为1分频 = 16Mhz
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);

    PWR_UltraLowPowerCmd(ENABLE);
    CLK_HaltConfig(CLK_Halt_FastWakeup, ENABLE);
    PWR_FastWakeUpCmd(ENABLE);


    CreateQueue(&Q);//创建队列
    CreateQueue(&Q_old);
    MyUart_Init();
    print_init_module(uart_txstring);
    initial_lcd();
    

    clear_screen();    //clear all dots
    display_128x64(bmp1);

    GPIO_Config();

//     ADCConver_Init();

//    u16 u16_adc1_value;
//    float adBattery = 0.0;
////    mini_sprint(buf,20,"mini print test\r\n");
////    uart_txstring(buf);
////    uart_txHex(0xff);
//    //uart_txarr(help,3,0);
//
//    while(1) {
//        CheckPower();
//        u16_adc1_value = ReadBattery();
//        mini_sprint(buf, 20, "value = %d\r\n", u16_adc1_value);
//        uart_txstring(buf);
//        adBattery = 2.8 * u16_adc1_value / 4096;
//        sprintf(buf, "%.3f \r\n", adBattery);
//        //mini_sprint(buf,20,"%.3f \r\n",adBattery);
//        uart_txstring(buf);
//        delayms(300);
//    }
    /*****************/



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
    
    if(GPIO_ReadInputDataBit(ADCPORT, ADCPIN) == SET){   //如果是插着充电器开机
        Power_charge = 1;       
    }
    TIM3_Init(); //初始化一下汇报充电状态定时器
    
    while(1) {
      
        UartScan();
        
//        if( (GPIO_ReadInputDataBit(AD_PORT, AD_PIN) == SET) && (LowPower==0) ){ //低电量,只通知一次
//             LowPower=1;
//             MAIN_LEN_OFF;//关闭PD7,关闭主灯
//            if(led_on) {             //息屏状态点亮屏幕
//                led_on = 0;
//                OLED_Display_On();
//                screen_off_cnt = 0;     //重置屏幕熄灭时间
//            }
//            screen_off_cnt = 0;
//            clear_screen();
//            display_GB2312_string(0, 32, "电量过低");            
//        }
      
        if(Power_charge) {          //只显示一次
            if(led_on) {             //息屏状态点亮屏幕
                led_on = 0;
                OLED_Display_On();
                screen_off_cnt = 0;     //重置屏幕熄灭时间
            }
//            MAIN_LEN_ON;//打开主灯控制功能
            LowPower=0;
            screen_off_cnt = 0;
            clear_screen();
            mini_sprint(buf, 20, "充电器已连接");           
            display_GB2312_string(0, 16, buf);
//            uart_txstring("充\r\n");                                   
            TIM3_Conut = 0;
            TIM3_Init(); //初始化一下汇报充电状态定时器
            uart_txarr(&PowerMsg[0], 1, 1);
                       
            while(Power_charge) {       //充电情况下屏幕不熄灭，一直检测充电断开，并检测是否进入测试模式
                UartScan();
                power_on = GPIO_ReadInputDataBit(ADCPORT, ADCPIN);
                if(power_on == 0) {                  

                    asm("sim");//close IT
                    Power_charge = 0; 
                    EXTI_SetPinSensitivity(ADC_EXTI_PIN, EXTI_Trigger_Rising); //上升沿中断                   
                    GPIO_Init(ADCPORT, ADCPIN, GPIO_Mode_In_FL_IT);//初始充电接口
                    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
                    asm("rim");
                    keyPassValue = 0;//断开之后重置按键状态
                    beep = 0;
                    memset(arr,0x00,49);
                    mini_sprint(buf, 20, "充电器已断开");
                    LED_GREEN_OFF;
                    LED_RED_OFF;  
                    clear_screen();
                    display_GB2312_string(0, 16, buf);
//                    uart_txstring("断\r\n");
                    TIM3_Conut = 0;
                    TIM3_Init(); //初始化一下汇报充电状态定时器                    
                    uart_txarr(&PowerMsg[1], 1, 1);
                    break;
                }
                keyPassValue = keyScan2();
                if(keyPassValue == KeyPassLong) {
                  if(beep == 1){
                    mini_sprint(buf, 20, "测试结束");
                    clear_screen();
                    display_GB2312_string(0, 32, buf);
                    beep = 0;
                    LED_GREEN_OFF;
                    LED_RED_OFF;  
                    uart_txarr(&PowerMsg[3], 1, 1);
                  }else{
                    mini_sprint(buf, 20, "产测模式");
                    clear_screen();
                    display_GB2312_string(0, 32, buf);                    
                    mini_sprint(arr, 49, "请注意观察上位机数据,LED和蜂鸣器是否正常!!");
                    display_GB2312_string(2, 0, arr);
                    beep = 1;
                    LED_GREEN_ON; 
                    LED_RED_ON;   
                    uart_txarr(&PowerMsg[2], 1, 1);
                  }
                }
                if((beep > 0) && (KEY_NORMAL == KeyRead())) {  //有按键操作不再响铃
                    sound2();
                }
                delayms(10);
            }
        }
        switch(AppState) {
            case NORMAL:
                if((LedF5 == 1) && (led_on == 0)) {
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    mini_sprint((char *)buf, 20, "收到%d条新消息！", num);
                    display_GB2312_string(0, 1, buf);
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        if(led_on) {             //息屏状态，单次按键，初次按下时点亮屏幕，不读消息
                            led_on = 0;
                            OLED_Display_On();
                            screen_off_cnt = 0;     //重置屏幕熄灭时间
//                            if(GPIO_ReadInputDataBit(AD_PORT, AD_PIN) == SET){ //低电量提醒，需按键按亮屏幕
//                              if(LedF5==0){//等到没有新消息刷新的时候再显示
//                                display_GB2312_string(0, 32, "电量过低");
//                              }
//                            }                           
                        } else {                  //非单次按下，读消息                                                     
                            if(num > 0) {   //如果有新消息
                                num--;         //读一次 信息次数--
                                num_r++;        //已读数++
                            } else {
                                num_r = 0;
                            }
                             
                            if(DeQueue(&Q, &pack)) {        //如果消息队列中有消息，出队，顺便入历史队
                                callID = get_pack(&pack, arr);
                                if(!EnQueue(&Q_old, &pack)) {   //历史队满了，进不去
                                    DeQueue(&Q_old, &pack_temp);    //丢弃最老的
                                    EnQueue(&Q_old, &pack);
                                }
                                clear_screen();    //clear all dots
                                mini_sprint((char *)buf, 20, "消息(%d)未读(%d)", num_r, num);
                                display_GB2312_string(0, 1, buf);
                                display_GB2312_string(2, 1, arr);
                                num_l = 0;      //读过新消息历史消息重置
                                ReadMsg[1] = (callID & 0xff00) >> 8;
                                ReadMsg[2] = callID;
                                uart_txarr(ReadMsg, 3, 1);   //暂定把已读功能放在这里
                            } else {                            //如果没有新消息读历史消息
                                if (size_queue(&Q_old) > 0) {
                                    num_l++;
                                    if(num_l > size_queue(&Q_old)) {
                                        num_l = 1;
                                    }
                                    pos = (Q_old.front + num_l) % (size_queue(&Q_old) + 1);//一个转圈循环算法，是一队头为基，num_l为加数
//                                    mini_print("f=%d pos=%d\r\n",Q_old.front,pos);
                                    get_pack(&(Q_old.Pack[pos]), arr);
                                    clear_screen();    //clear all dots
                                    mini_sprint((char *)buf, 20, "历史消息(%d)", num_l);
                                    display_GB2312_string(0, 1, buf);
                                    display_GB2312_string(2, 1, arr);
                                } else {
                                    clear_screen();    //clear all dots
                                    mini_sprint((char *)buf, 20, "无消息!");
                                    display_GB2312_string(0, 1, buf);
                                }
                            }
                            screen_off_cnt = 0;     //读了消息就重置屏幕熄灭时间
                        }
                        break;

                    case KeyPassLong :
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "求救成功!");
                        uart_txarr(&HelpMsg[0], 1, 1);
                        beep = 1;
                        display_GB2312_string(0, 1, buf);
                        if(LedF5 == 1) {
                            mini_sprint((char *)buf, 20, "收到%d条新消息!", num);
                            display_GB2312_string(2, 1, buf);
                            LedF5 = 0;
                        }
                        beep = 1;
                        screen_off_cnt = 0;     //重置屏幕熄灭时间
                        AppState = CALLING;
                        break;

                    default :
                        break;
                }
                if(AppState == NORMAL) {
                    delayms(10);
                    screen_off_cnt++;
                    if(screen_off_cnt > OFF_TIME) {
                        screen_off_cnt = 0;
                        OLED_Display_Off();
                        led_on = 1;
                       /* CheckPower();*/
//                        EnterHaltSleep();
//                        delayms(20);
//                        ExitHaltSleep();
//                        uart_txstring("exit sleep\r\n");
                    }
                }
                break;

            case WORNING:
                if(led_on) {             //息屏状态，点亮屏幕，并且不息屏
                    led_on = 0;
                    OLED_Display_On();
                }
                if(LedF5 == 1) {             //息屏状态，点亮屏幕
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    mini_sprint((char *)buf, 20, "收到%d条新消息!", num);
                    display_GB2312_string(0, 1, buf);
                    screen_off_cnt = 0;     //重置屏幕熄灭时间
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "请长按解除警报!");
                        display_GB2312_string(0, 1, buf);
                        mini_sprint((char *)buf, 20, "然后再读取消息!");
                        display_GB2312_string(2, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "警报已解除!");
                        display_GB2312_string(0, 1, buf);
                        uart_txarr(&HelpMsg[1], 1, 1);
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
                    delayms(10);
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
                    mini_sprint((char *)buf, 20, "收到%d条新消息", num);
                    display_GB2312_string(0, 1, buf);
                    screen_off_cnt = 0;     //重置屏幕熄灭时间
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "长按解除警报!");
                        display_GB2312_string(0, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "警报已解除!");
                        display_GB2312_string(0, 1, buf);
                        uart_txarr(&HelpMsg[1], 1, 1);
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
                    delayms(10);
                }
                break;
            default:
                break;
        }
        
    }
}

/*
u16 ReadBattery(void)
{
    ADC_SoftwareStartConv(ADC1);      //启动ADC

    while(ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == 0);  //等待转换结束
    ADC_ClearFlag(ADC1 , ADC_FLAG_EOC);                 //清除中断标志
    return ADC_GetConversionValue(ADC1);           //读取ADC1，通道1的转换结果
}

void CheckPower(void)
{
//    static u8 txLowPowerMsg = 1;
//    static u8 txChargeingMsg = 1;
//    static u8 txChargeOverMsg = 1;
    u8 powerMsg[3] = {0xfc, 0xfb, 0xfa};
    u16 u16_adc1_value[6];
    for(u8 i = 0; i < 5; i++) {
        u16_adc1_value[i] = ReadBattery();
    }
    u16 value = u16_adc1_value[2];
    if(value < LOW_POWER ) {
        uart_txarr(&powerMsg[0], 1, 1);
    } else if(value < CHARGE_OVER) {
        uart_txarr(&powerMsg[2], 1, 1);
    } else if(value > CHARGEING) {
        uart_txarr(&powerMsg[1], 1, 1);
    }
}
*/
