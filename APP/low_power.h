#ifndef __LOW_POWER_H
#define __LOW_POWER_H
#include "stm8l15x.h"

#define USE_LPOWER  1   //启用低功耗功能

#define ADCPORT         GPIOB
#define ADCPIN          GPIO_Pin_1
#define ADC_EXTI_PIN    EXTI_Pin_1


void EnterHaltSleep(void);
void ExitHaltSleep(void);
void ADCConver_Init(void);

#endif
