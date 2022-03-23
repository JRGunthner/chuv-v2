#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include "stm32f0xx.h"

void PID_Task(void);

//-------------------------------
// Functions Definitions
uint16_t Controller_Task(uint16_t SetPoint);
void     Controller_ResetPID(void);
uint16_t Controller_GetSetPoint();

#endif /* __CONTROLLER_H */
