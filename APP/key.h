#ifndef __KEY_H
#define __KEY_H

#include "stm8l15x.h"

#define KEY_LONG_PASS_TIME 700
#define TEST_TIME          800  //产测时间

#define KEY_NORMAL         0X40
#define KEYPORT  GPIOD
#define KEYPIN   GPIO_Pin_6

#define LED_GREEN_PORT  GPIOA
#define LED_GREEN_PIN   GPIO_Pin_5
#define LED_RED_PORT    GPIOA
#define LED_RED_PIN     GPIO_Pin_4

//版本2
#define BANBEN2         0

#if BANBEN2
#define PWM_PORT        GPIOB
#define PWM_PIN         GPIO_Pin_4

#define MAIN_LED_PORT   GPIOD
#define MAIN_LED_PIN    GPIO_Pin_7
#define MAIN_LED_OFF    GPIO_ResetBits(MAIN_LED_PORT,MAIN_LED_PIN)
#define MAIN_LED_ON     GPIO_SetBits(MAIN_LED_PORT,MAIN_LED_PIN)

#define AD_PORT         GPIOB
#define AD_PIN          GPIO_Pin_2
#endif


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
unsigned char PowerScan(uint8_t level);
#endif
