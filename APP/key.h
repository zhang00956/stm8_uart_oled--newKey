#ifndef __KEY_H
#define __KEY_H

#include "stm8l15x.h"

#define KEY_LONG_PASS_TIME 50
#define TEST_TIME          800  //²ú²âÊ±¼ä

#define KEY_NORMAL         0X40
#define KEYPORT  GPIOD
#define KEYPIN   GPIO_Pin_6

#define LED_GREEN_PORT  GPIOA
#define LED_GREEN_PIN   GPIO_Pin_5
#define LED_RED_PORT    GPIOA
#define LED_RED_PIN     GPIO_Pin_4

#define LED_GREEN_ON     GPIO_ResetBits(LED_GREEN_PORT,LED_GREEN_PIN)
#define LED_RED_ON       GPIO_ResetBits(LED_RED_PORT,LED_RED_PIN)
#define LED_GREEN_OFF    GPIO_SetBits(LED_GREEN_PORT,LED_GREEN_PIN)
#define LED_RED_OFF      GPIO_SetBits(LED_RED_PORT,LED_RED_PIN)    


enum _KEY_STATUS {
    waitForPass = 0,
    keyPassOne,
    KeyPassLong,
    keyPassReturn,
};
void GPIO_Config(void);
void BeepInit(void);
void LEDInit(void);
unsigned char keyScan(void);
unsigned char keyScan2(void);
unsigned char KeyRead( void );
#endif
