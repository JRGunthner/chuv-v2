#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f0xx.h"

//----------------------------------------------
// Defines for Display_UpdatePoints function
#define DISPLAY_POINT_MIDDLE 0x01
#define DISPLAY_POINT_RIGHT  0x10

//-------------------------------
// Functions Definitions
void Display_Init(void);
void Display_Task(void);

void Display_UpdateChar(uint8_t CharLeft, uint8_t CharRight);
void Display_UpdateChar2(char *Char);
void Display_UpdateDecimal(uint8_t DecimalDigitLeft, uint8_t DecimalDigitRight);
void Display_UpdateValue(int32_t Value); // TODO: Colocar comentario
void Display_UpdateTimeValue(uint32_t Value);

/**
  * @brief  Turn points on or off in displays.
  * @param	Value: this parameter can be set to 0 to DOT ON or 1 to DOT OFF.
  * @param	Point: specifies what point is enabled or disabled.
  * 				This parameter can be  one of the following values:
  * 					@arg DISPLAY_POINT_MIDDLE: to middle point
  * 					@arg DISPLAY_POINT_RIGHT: to right point
  * @retval None
  */
void Display_UpdatePoints(uint8_t Value, uint8_t Point);

/**
  * @brief  Print Firmware Version in displays
  * @param	Major: this parameter must be set to a value in the 0-9 range.
  * 					It's written in Right Display.
  * @param	Minor: this parameter must be set to a value in the 0-9 range.
  * 					It's written in Left Display.
  * @retval None
  */
void Display_Version(uint8_t Major, uint8_t Minor);

#endif/*__DISPLAY_H */
