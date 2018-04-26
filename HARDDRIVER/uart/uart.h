#ifndef _UART_H
#define _UART_H

#include "stm8l15x.h"

enum __AppState{
    NORMAL = 0,
    CALLING,     //Çó¾È
    WORNING,
    POWER_ING,      //³äµç
    POWER_OFF
};

void MyUart_Init(void);
void UART1_SendString(char *buf);
void UART1_SendNumber(int num);
uint8_t UartScan(void);

void uart_txstring(uint8_t *p);
void uart_txarr(uint8_t *p,uint16_t len,uint8_t endFlag);
void uart_txHex(unsigned char dat);

#endif