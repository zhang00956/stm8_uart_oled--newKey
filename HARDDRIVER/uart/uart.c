#include "uart.h"
#include "stdio.h"
#include "stm8l15x_it.h"
#include "string.h"
#include "queue.h"
#include "LCD_ZK.h"
#include "timer.h"
#include "key.h"
#include "mini-printf.h"

//用户代码的起始地址
#define MAIN_USER_Start_ADDR     ((uint32_t)0x8000+0X2000)

uint8_t HexTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char num = 0; //消息未读数量
extern char num_r;
extern queue Q;
extern u8 AppState;
extern u8 beep;
extern u8 LedF5;
extern volatile u8 led_on;
extern volatile u16 screen_off_cnt;
extern volatile u8 tim_ack;
void MyUart_Init(void)
{

//    USART_DeInit(USART1);
    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT); //PC2 RX
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //PC3  TX

    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //使能串口时钟

    USART_Init(USART1, 115200, USART_WordLength_8b, USART_StopBits_1,
               USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能接受中断
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    //USART_ITConfig(USART1,USART_IT_OR, DISABLE);//atention
    USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
    USART_ITConfig(USART1, USART_IT_ERR, DISABLE);

    USART_Cmd(USART1, ENABLE);
// memset(USART_RX_BUF,0x00,USART_REC_LEN);
}
void UartSendByte(unsigned char dat)
{
while(((USART1->SR) & 0x80) == 0x00);  
    USART1->DR = dat;
    
}
void uart_txstring(uint8_t *p)
{
    while(*p != '\0') {
        UartSendByte(*p++);
    }
}
void uart_txarr(uint8_t *p, uint16_t len, uint8_t endFlag)
{
    uint16_t i;
    for(i = 0; i < len; i++) {
        UartSendByte(p[i]);
    }
    if(endFlag) {
        UartSendByte(0x0d);
        UartSendByte(0x0a);
    }
}

void uart_txHex(unsigned char dat)
{
    UartSendByte('0');
    UartSendByte('x');
    UartSendByte(HexTable[dat >> 4]);
    UartSendByte(HexTable[dat & 0x0f]);
    UartSendByte(' ');

}
void UART1_SendString(char buf[])
{
    unsigned int i = 0;
    while(buf[i] != '\0') {

        while(((USART1->SR) & 0x80) == 0x00);
        USART_SendData8(USART1, buf[i++]);
    }
}

void UART1_SendNumber(int num)
{
    char buf[20];//顶一个局部缓冲区

    sprintf((char *)buf, "%d\n", num);

    UART1_SendString(buf);
}

////重定向
//int fputc(int ch, FILE *f)
//{
//    while(((USART1->SR) & 0x80) == 0x00);
//    USART_SendData8(USART1, ch);
//    return ch;
//}

typedef void (*VOIDFUN)();   
const VOIDFUN RestMain =(VOIDFUN) 0X8000;

uint8_t UartScan(void)
{

    u8 len;
    u8 ack = 0xf1;   
    u16 zdID = 0;
//    u8 t;    
    u8 buf[20];//顶一个局部缓冲区
    extern u16 screen_off_cnt;
    SNode pack;
    //u8 arr[49];
    if(USART_RX_STA & 0x8000) {
        len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度

//			for(t=0;t<len;t++)
//			{
//                USART_SendData8(USART1,USART_RX_BUF[t]);
//                while(((USART1->SR) & 0x80) == 0x00);
//			}   
        if(len < 4) {
            if(USART_RX_BUF[0] == 0x05)
              uart_txarr(&ack,1,1);//回复F1
            screen_off_cnt = 0;
            USART_RX_STA = 0;
            return 0;
        }
        switch (USART_RX_BUF[0]) {
                /*
                            case CHE_LI :
                                beep = 1;
                                TIM2_Init();
                                num++;
                                num_r = 0;
                                if(num > (FIFO_SIZE - 1)) {
                                    num = (FIFO_SIZE - 1);
                                    DeQueue(&Q, &pack);
                                }
                                set_pack(&USART_RX_BUF[1], &pack, len - 3);
                                EnQueue(&Q, &pack);
                                AppState = WORNING;
                                 LedF5 = 1;
                                break;
                            case FEI_CHE_LI :
                                beep = 1;
                                TIM2_Init();

                                num++;
                                num_r = 0;
                                if(num > (FIFO_SIZE - 1)) {
                                    num = (FIFO_SIZE - 1);
                                    DeQueue(&Q, &pack);
                                }
                                set_pack(&USART_RX_BUF[1], &pack, len - 3);
                                EnQueue(&Q, &pack);
                                AppState = WORNING;
                                 LedF5 = 1;
                                break;
                          */
            case CALL_REN :
                beep = 1;
                TIM2_Init();

                num++;
                num_r = 0;
                if(num > (FIFO_SIZE - 1)) {
                    num = (FIFO_SIZE - 1);
                    DeQueue(&Q, &pack);
                }

                set_pack(&USART_RX_BUF[1], &pack, len - 3);
                EnQueue(&Q, &pack);
                AppState = WORNING;
                LedF5 = 1;
                break;
            case NORMAL_MSG:
                if(*(u16*)&USART_RX_BUF[1]) {
                    num++;
                    num_r = 0;
                    if(num > (FIFO_SIZE - 1)) {
                        num = (FIFO_SIZE - 1);
                        DeQueue(&Q, &pack);
                    }

                    set_pack(&USART_RX_BUF[1], &pack, len - 3);
                    EnQueue(&Q, &pack);
                    //TIM2_Init();
                    if(AppState == NORMAL) {
                        AppState = NORMAL;
                    }
                    LedF5 = 1;
                } else { //如果是第0号消息
                    if(led_on) {             //息屏状态，点亮屏幕
                        led_on = 0;
                        OLED_Display_On();
                    }
                    screen_off_cnt = 0;
                    if(USART_RX_BUF[3] == 0xfc) {
                        zdID = (USART_RX_BUF[4]<<8) | USART_RX_BUF[5];
                        clear_screen();
                        mini_sprint((char *)buf, 20, "ID:%u", zdID);
                        display_GB2312_string(0, 32, "充电中");
                        display_GB2312_string(2, 32, buf);
                        tim_ack = 1;
                        
                        TIM3_Cmd(DISABLE);
                    } else if(USART_RX_BUF[3] == 0xfb) {
                        clear_screen();
                        display_GB2312_string(0, 32, "充电结束");
                        tim_ack = 1;
                        
                        TIM3_Cmd(DISABLE);
                    }else if(USART_RX_BUF[3] == 0xAC && USART_RX_BUF[4]==0XAC){
                          FLASH_Unlock(FLASH_MemType_Program);
                          FLASH_EraseByte(MAIN_USER_Start_ADDR-1);    //清除APP标记
                          RestMain();   //复位
                    
                    } else {
                        clear_screen();
                        display_GB2312_string(0, 1, &USART_RX_BUF[3]);
                    }
                }

                break;
            default :
                break;
        }
        USART_RX_STA = 0;
        memset(USART_RX_BUF, 0x00, USART_REC_LEN);
        LEDInit();      
    }
    return 0;
}

