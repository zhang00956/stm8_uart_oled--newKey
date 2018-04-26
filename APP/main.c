//////////////////////////////////////////////////////////////////////////////////
//  ��������   : �ֿ�� OLED SPI�ӿ���ʾ����(STM8Lϵ��)
//              ˵��:
//              ------------------------����ΪOLED��ʾ���õ��Ľ�?---------------------------------------
//              GND    ��Դ��
//              VCC   ��5V��3.3v��Դ
//              CLK   PB3��CLK��
//              MOSI  PD1��DIN��
//              DC    PD2 ��data or cmd��
//              CS1   PD3
//              FSO   PD4  (MISO)
//              CS2   PD5
//����������it.h����
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

#define OFF_TIME           500



queue Q, Q_old;
extern u8 num;//δ��
char num_r = 0;//�Ѷ�
char num_l = 0;//��ʷ��Ϣ
//u16 key_num = 0; //��������
//u8 flag_next = 1;//��������ִ��
volatile u8 led_on = 0;//��һ�ΰ�����������Ļ
volatile u8 AppState = NORMAL;//Ĭ������״̬
volatile u8 LedF5 = 0;   //�Ƿ�ˢ����ʾ�յ���Ϣ
volatile u8 beep = 0;//����������
volatile u8 keyPassValue = 0; //�����̰�״̬
volatile u8 Power_charge = 0; //���������״̬���

/*0xff�����Ϣ
 *0xfeȡ������
 *0xfd�Ѷ���Ϣ
 */
u8 HelpMsg[3] = {0xff, 0xfe, 0xfd};
u8 PowerMsg[4] = {0xfc, 0xfb, 0xfa, 0xf9};
void ADCConver_Init(void);
u16 ReadBattery(void);
void CheckPower(void);

int main(void)
{
    u8 power_on = 0;

    u8 arr[49]={0};
    SNode pack, pack_temp;
    u8 buf[20] = {0x00}; //��һ���ֲ�������
    u16 cnt_t = 0; //Ϣ����ʱ
    u8 pos = 0;   //��Ϣ����λ��

    CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���л�
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//ѡ���ⲿ����ʱ����Ϊʱ��Դ
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);

    PWR_UltraLowPowerCmd(ENABLE);
    CLK_HaltConfig(CLK_Halt_FastWakeup, ENABLE);
    PWR_FastWakeUpCmd(ENABLE);


    CreateQueue(&Q);//��������
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
//    transfer_command_lcd(0x00);//��ʼ��ַ
//    transfer_command_lcd(0x00);//����ʱ��
//    transfer_command_lcd(0x07);//������ַ
//    transfer_command_lcd(0x00);
//    transfer_command_lcd(0xFF);
//    transfer_command_lcd(0x2F);//��ʼ����
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
    if(GPIO_ReadInputDataBit(ADCPORT, ADCPIN) == (uint8_t)ADCPIN){   //����ǲ��ų��������
        Power_charge = 1;
    }
    while(1) {
        if(Power_charge) {          //ֻ��ʾһ��
            if(led_on) {             //Ϣ��״̬������Ļ
                led_on = 0;
                OLED_Display_On();
                cnt_t = 0;     //������ĻϨ��ʱ��
            }
            cnt_t = 0;
            clear_screen();
            mini_sprint(buf, 20, "�����������");
            display_GB2312_string(0, 16, buf);
            uart_txstring("��\r\n");
            uart_txarr(&PowerMsg[0], 1, 1);
            while(Power_charge) {       //����������Ļ��Ϩ��һֱ�����Ͽ���������Ƿ�������ģʽ
                power_on = GPIO_ReadInputDataBit(ADCPORT, ADCPIN);
                if(power_on == 0) {                  

                    asm("sim");//close IT
                    Power_charge = 0; 
                    EXTI_SetPinSensitivity(ADC_EXTI_PIN, EXTI_Trigger_Rising); //�������ж�                   
                    GPIO_Init(ADCPORT, ADCPIN, GPIO_Mode_In_FL_IT);//��ʼ���ӿ�
                    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
                    asm("rim");
                    keyPassValue = 0;//�Ͽ�֮�����ð���״̬
                    beep = 0;
                    memset(arr,0x00,49);
                    mini_sprint(buf, 20, "������ѶϿ�");
                    clear_screen();
                    display_GB2312_string(0, 16, buf);
                    uart_txstring("��\r\n");
                    uart_txarr(&PowerMsg[1], 1, 1);
                    break;
                }
                keyPassValue = keyScan2();
                if(keyPassValue == KeyPassLong) {
                  if(beep == 1){
                    mini_sprint(buf, 20, "���Խ���");
                    clear_screen();
                    display_GB2312_string(0, 32, buf);
                    beep = 0;
                    LED_GREEN_OFF;
                    LED_RED_OFF;  
                    uart_txarr(&PowerMsg[3], 1, 1);
                  }else{
                    mini_sprint(buf, 20, "����ģʽ");
                    clear_screen();
                    display_GB2312_string(0, 32, buf);                    
                    mini_sprint(arr, 49, "��ע��۲���λ������,LED�ͷ������Ƿ�����!!");
                    display_GB2312_string(2, 0, arr);
                    beep = 1;
                    LED_GREEN_ON; 
                    LED_RED_ON;   
                    uart_txarr(&PowerMsg[2], 1, 1);
                  }
                }
                if((beep > 0) && (KEY_NORMAL == KeyRead())) {  //�а���������������
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
                    mini_sprint((char *)buf, 20, "�յ�%d������Ϣ��", num);
                    display_GB2312_string(0, 1, buf);
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        if(led_on) {             //Ϣ��״̬�����ΰ��������ΰ���ʱ������Ļ��������Ϣ
                            led_on = 0;
                            OLED_Display_On();
                            cnt_t = 0;     //������ĻϨ��ʱ��
                        } else {                  //�ǵ��ΰ��£�����Ϣ
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
                                mini_sprint((char *)buf, 20, "��Ϣ(%d)δ��(%d)", num_r, num);
                                display_GB2312_string(0, 1, buf);
                                display_GB2312_string(2, 1, arr);
                                num_l = 0;
                                uart_txarr(&HelpMsg[2], 1, 1);   //�ݶ����Ѷ����ܷ�������
                            } else {
                                if (size_queue(&Q_old) > 0) {
                                    num_l++;
                                    if(num_l > size_queue(&Q_old)) {
                                        num_l = 1;
                                    }
                                    pos = (Q_old.front + num_l) % (size_queue(&Q_old) + 1);
                                    get_pack(&(Q_old.Pack[pos]), arr);
                                    clear_screen();    //clear all dots
                                    mini_sprint((char *)buf, 20, "��ʷ��Ϣ(%d)", num_l);
                                    display_GB2312_string(0, 1, buf);
                                    display_GB2312_string(2, 1, arr);
                                } else {
                                    clear_screen();    //clear all dots
                                    mini_sprint((char *)buf, 20, "����Ϣ!");
                                    display_GB2312_string(0, 1, buf);
                                }
                            }
                            cnt_t = 0;     //������Ϣ��������ĻϨ��ʱ��
                        }
                        break;

                    case KeyPassLong :
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "��ȳɹ�!");
                        uart_txarr(&HelpMsg[0], 1, 1);
                        beep = 1;
                        display_GB2312_string(0, 1, buf);
                        if(LedF5 == 1) {
                            mini_sprint((char *)buf, 20, "�յ�%d������Ϣ!", num);
                            display_GB2312_string(2, 1, buf);
                            LedF5 = 0;
                        }
                        beep = 1;
                        cnt_t = 0;     //������ĻϨ��ʱ��
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
//                        CheckPower();
                        EnterHaltSleep();
                        delayms(20);
                        ExitHaltSleep();
                        uart_txstring("exit sleep\r\n");
                    }
                }
                break;

            case WORNING:
                if(led_on) {             //Ϣ��״̬��������Ļ�����Ҳ�Ϣ��
                    led_on = 0;
                    OLED_Display_On();
                }
                if(LedF5 == 1) {             //Ϣ��״̬��������Ļ
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    mini_sprint((char *)buf, 20, "�յ�%d������Ϣ!", num);
                    display_GB2312_string(0, 1, buf);
                    cnt_t = 0;     //������ĻϨ��ʱ��
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "�볤���������!");
                        display_GB2312_string(0, 1, buf);
                        mini_sprint((char *)buf, 20, "Ȼ���ٶ�ȡ��Ϣ!");
                        display_GB2312_string(2, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "�����ѽ��!");
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
                    if((beep > 0) && (KEY_NORMAL == KeyRead())) {  //�а���������������
                        sound2();
                    }
                    delayms(20);
                }
                break;
            case CALLING:
                if(led_on) {             //Ϣ��״̬��������Ļ�����Ҳ�Ϣ��
                    led_on = 0;
                    OLED_Display_On();
                }
                if(LedF5 == 1) {             //Ϣ��״̬��������Ļ
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    mini_sprint((char *)buf, 20, "�յ�%d������Ϣ", num);
                    display_GB2312_string(0, 1, buf);
                    cnt_t = 0;     //������ĻϨ��ʱ��
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "�����������!");
                        display_GB2312_string(0, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        mini_sprint((char *)buf, 20, "�����ѽ��!");
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
                    if((beep > 0) && (KEY_NORMAL == KeyRead())) {  //�а���������������
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

/*
u16 ReadBattery(void)
{
    ADC_SoftwareStartConv(ADC1);      //����ADC

    while(ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == 0);  //�ȴ�ת������
    ADC_ClearFlag(ADC1 , ADC_FLAG_EOC);                 //����жϱ�־
    return ADC_GetConversionValue(ADC1);           //��ȡADC1��ͨ��1��ת�����
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
