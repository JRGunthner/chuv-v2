#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f0xx.h"

//-------------------------------
// Buzzer_Control Function Definitions
#define BUZZER_ENABLED 0x0F
#define BUZZER_DISABLE 0xF0

//-------------------------------
// Functions Definitions

/**
  * @brief
  * @param  None
  * @retval None
  */
void Buzzer_Init(void);

void Buzzer_Task(void);

void Buzzer_Beep(uint16_t Time, uint8_t Repeat);

void Buzzer_Control(uint8_t EnDis);

#endif /* __BUZZER_H */
