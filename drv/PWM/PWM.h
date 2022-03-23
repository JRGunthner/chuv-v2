#ifndef __PWM_H
#define __PWM_H

#include "stm32f0xx.h"

//-------------------------------
// Channel definitions for PWM_SetDutyCycle()
#define PWM_CHANNEL_LED_RED		1
#define PWM_CHANNEL_LED_GREEN	2
#define PWM_CHANNEL_LED_BLUE	3
#define PWM_CHANNEL_BUZZER      4

//-------------------------------
// Functions Definitions
void PWM_Init (void);
void PWM_SetDutyCycle (uint8_t Channel, uint8_t DutyCycle);

#endif
