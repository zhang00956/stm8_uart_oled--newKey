#ifndef __KEY_H
#define __KEY_H

#include "stm8l15x.h"

#define KEY_LONG_PASS_TIME 50
#define KEY_NORMAL         0X40
#define KEYPORT  GPIOD
#define KEYPIN   GPIO_Pin_6

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
unsigned char KeyRead( void );
#endif
