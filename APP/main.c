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
extern u8 num;//δ��
char num_r = 0;//�Ѷ�
char num_l = 0;//��ʷ��Ϣ
//u16 key_num = 0; //��������
//u8 flag_next = 1;//��������ִ��
u8 led_on = 0;//��һ�ΰ�����������Ļ
u8 AppState = NORMAL;//Ĭ������״̬
u8 LedF5 = 0;   //�Ƿ�ˢ����ʾ�յ���Ϣ
u8 beep = 0;//����������
u8 keyPassValue = 0; //�����̰�״̬
char help[4] = {0xff, 0x0d, 0x0a, 0x00}; //�����Ϣ
char CanHelp[4] = {0xfe, 0x0d, 0x0a, 0x00}; //ȡ������
char AllRead[4] = {0xfd, 0x0d, 0x0a, 0x00}; //�Ѷ�

void ADCConver_Init(void);
u16 ReadBattery(void);
int main(void)
{
    u8 arr[49];
    SNode pack, pack_temp;
    u8 buf[20]={0x00};//��һ���ֲ�������
    u16 cnt_t = 0; //Ϣ����ʱ
    u8 pos = 0;   //��Ϣ����λ��
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz
    CreateQueue(&Q);//��������
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
    
    uart_txstring("����\r\n");
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

    while(1) {
        switch(AppState) {
            case NORMAL:
                if((LedF5 == 1) && (led_on == 0)) {
                    LedF5 = 0;
                    clear_screen();    //clear all dots
                    sprintf((char *)buf, "�յ�%d������Ϣ��", num);
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
                                sprintf((char *)buf, "��Ϣ(%d)δ��(%d)", num_r, num);
                                display_GB2312_string(0, 1, buf);
                                display_GB2312_string(2, 1, arr);
                                num_l = 0;
                                UART1_SendString(AllRead);   //�ݶ����Ѷ����ܷ�������
                            } else {
                                if (size_queue(&Q_old) > 0) {
                                    num_l++;
                                    if(num_l > size_queue(&Q_old)) {
                                        num_l = 1;
                                    }
                                    pos = (Q_old.front + num_l) % (size_queue(&Q_old) + 1);
                                    get_pack(&(Q_old.Pack[pos]), arr);
                                    clear_screen();    //clear all dots
                                    sprintf((char *)buf, "��ʷ��Ϣ(%d)", num_l);
                                    display_GB2312_string(0, 1, buf);
                                    display_GB2312_string(2, 1, arr);
                                } else {
                                    clear_screen();    //clear all dots
                                    sprintf((char *)buf, "����Ϣ!");
                                    display_GB2312_string(0, 1, buf);
                                }
                            }
                            cnt_t = 0;     //������Ϣ��������ĻϨ��ʱ��
                        }
                        break;

                    case KeyPassLong :
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "��ȳɹ�!");
                        UART1_SendString(help);
                        beep = 1;
                        display_GB2312_string(0, 1, buf);
                        if(LedF5 == 1) {
                            sprintf((char *)buf, "�յ�%d������Ϣ��", num);
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
                    }
                }
                break;

            case WORNING:
                if(LedF5 == 1) {             //Ϣ��״̬��������Ļ
                    LedF5 = 0;
                    OLED_Display_On();
                    clear_screen();    //clear all dots
                    sprintf((char *)buf, "�յ�%d������Ϣ", num);
                    display_GB2312_string(0, 1, buf);
                    cnt_t = 0;     //������ĻϨ��ʱ��
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "�볤���������!");
                        display_GB2312_string(0, 1, buf);
                        sprintf((char *)buf, "Ȼ���ٶ�ȡ��Ϣ!");
                        display_GB2312_string(2, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "�����ѽ��!");
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
                    sprintf((char *)buf, "�յ�%d������Ϣ", num);
                    display_GB2312_string(0, 1, buf);
                    cnt_t = 0;     //������ĻϨ��ʱ��
                }
                keyPassValue = keyScan();
                switch (keyPassValue) {
                    case keyPassOne:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "�����������!");
                        display_GB2312_string(0, 1, buf);
                        break;
                    case KeyPassLong:
                        clear_screen();    //clear all dots
                        sprintf((char *)buf, "�����ѽ��!");
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

void ADCConver_Init(void)
{
    u8 idx;

    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1 , ENABLE);              //ʹ��ADC1ʱ��
    GPIO_Init(ADCPORT , ADCPIN , GPIO_Mode_In_FL_No_IT);  //����ADCPIN�������룬���жϽ�ֹ

    ADC_VrefintCmd(ENABLE);//ʹ���ڲ��ο���ѹ
    ADC_Init(ADC1,
             ADC_ConversionMode_Single,   //����ת��ģʽ
             ADC_Resolution_12Bit,        //12λ����ת��е
             ADC_Prescaler_2              //ʱ������Ϊ2��Ƶ
            );

    ADC_ChannelCmd(ADC1,
                   ADC_Channel_17,         //����Ϊͨ��17���в���
                   ENABLE);

    ADC_Cmd(ADC1 , ENABLE);               //ʹ��ADC
    for (idx = 0; idx < 50; idx++); //adc�ϵ���Ҫһ��ʱ��
}

u16 ReadBattery(void)
{
    ADC_SoftwareStartConv(ADC1);      //����ADC

    while(ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == 0);  //�ȴ�ת������
    ADC_ClearFlag(ADC1 , ADC_FLAG_EOC);                 //����жϱ�־
    return ADC_GetConversionValue(ADC1);           //��ȡADC1��ͨ��1��ת�����
}



