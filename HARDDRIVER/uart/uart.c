#include "uart.h"
#include "stdio.h"
#include "stm8l15x_it.h"
#include "string.h"
#include "queue.h"
#include "LCD_ZK.h"
#include "timer.h"

char num = 0; //��Ϣδ������
extern char num_r;
extern queue Q;
extern u8 AppState;
extern u8 beep;
extern u8 LedF5;

void MyUart_Init(void)
{

    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT); //PC2 RX
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //PC3  TX

    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //ʹ�ܴ���ʱ��

    USART_Init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1,
               USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //ʹ�ܽ����ж�
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    //USART_ITConfig(USART1,USART_IT_OR, DISABLE);//atention
    USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
    USART_ITConfig(USART1, USART_IT_ERR, DISABLE);

    USART_Cmd(USART1, ENABLE);
// memset(USART_RX_BUF,0x00,USART_REC_LEN);
    enableInterrupts();
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
    char buf[20];//��һ���ֲ�������

    sprintf((char *)buf, "%d\n", num);

    UART1_SendString(buf);
}

//�ض���
int fputc(int ch, FILE *f)
{
    while(((USART1->SR) & 0x80) == 0x00);
    USART_SendData8(USART1, ch);
    return ch;
}

void UartScan(void)
{

    u8 len;
//    u8 t;
//    u8 buf[10];//��һ���ֲ�������
    SNode pack;
    //u8 arr[49];
    if(USART_RX_STA & 0x8000) {
        len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
//			for(t=0;t<len;t++)
//			{
//                USART_SendData8(USART1,USART_RX_BUF[t]);
//                while(((USART1->SR) & 0x80) == 0x00);
//			}
        LedF5 = 1;
        switch (USART_RX_BUF[0]) {
            case CHE_LI :
                beep = 1;
                TIM2_Init();
                num++;
                num_r = 0;
                if(num > (FIFO_SIZE - 1)) {
                    num = (FIFO_SIZE - 1);
                    DeQueue(&Q, &pack);
                }
                set_pack(&USART_RX_BUF[3], &pack, len - 3);
                EnQueue(&Q, &pack);
                AppState = WORNING;
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
                set_pack(&USART_RX_BUF[3], &pack, len - 3);
                EnQueue(&Q, &pack);
                AppState = WORNING;
                break;
            case CALL_REN :
                beep = 1;
                TIM2_Init();

                num++;
                num_r = 0;
                if(num > (FIFO_SIZE - 1)) {
                    num = (FIFO_SIZE - 1);
                    DeQueue(&Q, &pack);
                }

                set_pack(&USART_RX_BUF[3], &pack, len - 3);
                EnQueue(&Q, &pack);
                AppState = WORNING;
                break;
            default :
                num++;
                num_r = 0;
                if(num > (FIFO_SIZE - 1)) {
                    num = (FIFO_SIZE - 1);
                    DeQueue(&Q, &pack);
                }

                set_pack(&USART_RX_BUF[3], &pack, len - 3);
                EnQueue(&Q, &pack);
                //TIM2_Init();
                if(AppState == NORMAL){
                    AppState = NORMAL;
                }
                
                break;
        }
        USART_RX_STA = 0;
//            memset(USART_RX_BUF,0x00,USART_REC_LEN);
    }
}
