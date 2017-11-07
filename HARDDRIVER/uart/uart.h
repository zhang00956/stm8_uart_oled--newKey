#ifndef _UART_H
#define _UART_H

#include "stm8l15x.h"

enum __AppState{
    NORMAL = 0,
    CALLING,     //���
    WORNING,
};

void MyUart_Init(void);
void UART1_SendString(char *buf);
void UART1_SendNumber(int num);
void UartScan(void);
#endif