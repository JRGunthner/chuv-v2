#ifndef __DEBUG_H
#define __DEBUG_H

#include "stm32f0xx.h"

//-------------------------------
// Functions Definitions
void Debug_Init(void);
void Debug_Write(char *Data, uint32_t Tam);

#endif /* __DEBUG_H */
