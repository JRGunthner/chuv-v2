#ifndef __AD_H
#define __AD_H

#include "stm32f0xx.h"

//-------------------------------
// AD_GetValue() defines
#define AD_VOLTMETER	                 0
#define AD_NTC1			                 1
#define AD_NTC2			                 2
#define AD_NTC3			                 3
#define AD_TOUCH_UP		                 4
#define AD_TOUCH_DOWN	                 5
#define AD_PROCESSOR_TEMPERATURE_SENSOR  6
#define AD_VOLTAGE_REFERENCE             7

//-------------------------------
// Functions Definitions

/**
  * @brief	Initialize Analog to Digital Converter
  * @param  None
  * @retval None
  */
void AD_Init (void);

/**
  * @brief
  * @param  None
  * @retval None
  */
uint16_t AD_GetValue(uint8_t index);

#endif/* __AD_H */
