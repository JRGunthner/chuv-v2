#ifndef __TRIAC_H
#define __TRIAC_H

#include "stm32f0xx.h"

//----------------------------------------------
// Defines for uint8_t TRIAC_GetError();
#define TRIAC_OK               0
#define TRIAC_ERROR_T1         1
#define TRIAC_ERROR_T2         2
#define TRIAC_ERROR_T3         3
#define TRIAC_ERROR_LOST_EDGES 4

//-------------------------------
// Functions Definitions
void TRIAC_Init(void);
void TRIAC_EnableIRQ(void);
void TRIAC_DisableIRQ(void);

/**
  * @brief
  * @param  Power: this parameter must be set to a value in the 0-100 range.
  * @retval None
  */
void    TRIAC_SetPower(uint8_t Power);

/**
  * @brief
  * @param  None
  * @retval Power
  */
uint8_t TRIAC_GetPower(void);

uint8_t TRIAC_GetError();
void    TRIAC_LostEdges();

#endif// __TRIAC_H
