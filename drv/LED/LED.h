#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

//-------------------------------
// User Led Mode used by function LED_SetMode(uint8_t LedMode)
#define LED_OFF			 0x01  // Led OFF
#define LED_TEMPERATURE  0x02  // Led color changes by NTC outlet temperature
#define LED_POWER		 0x03  // Led color changes by Triac Power
#define LED_AUTO	     0x04  // Led color changes automatically between all led colors definitions
#define LED_FIXED		 0x05  // Led color is set by LED_SetColor(uint8_t LedColor) function

//-------------------------------
// LED color Definitions for LED_SetColor(uint8_t LedColor) and returned by LED_GetColor() functions
#define LED_COLOR_OFF			0x00
#define LED_COLOR_WHITE    	    0x01
#define LED_COLOR_LIGHT_BLUE  	0x02
#define LED_COLOR_BLUE  		0x03
#define LED_COLOR_LIGHT_GREEN  	0x04
#define LED_COLOR_GREEN  		0x05
#define LED_COLOR_YELLOW  		0x06
#define LED_COLOR_ORANGE  		0x07
#define LED_COLOR_RED		  	0x08
#define LED_COLOR_PINK		  	0x09

//-------------------------------
// Time that LED color will change when LED_MODE_AUTO is set
#define LED_TIME_TO_CHANGE 450 // 4,5s

//-------------------------------
// LED_Enable function definitions
#define LED_ENABLED 0x01
#define LED_DISABLE 0x02

//-------------------------------
// Time that API will wait to disable leds after calling LED_Enable function
#define LED_TIME_TO_DISABLE 1000 // 10s

//-------------------------------
// Functions Definitions
void    LED_Init(void);                       // Initialization routine (basically PWM initialization)
void    LED_Task();                           // Function that controls API State Machine and must be called every 10ms (Not hard real Time)
void    LED_Control(uint8_t EnaDis);          // Will turn LEDs available or disable (This is done because the leds will be available only when there is water flow)
void    LED_SetMode(uint8_t Led_Mode);        // Changes LED behavior (State Machine)
void    LED_SetFixedColor(uint8_t Led_Color); // SetColor function works only when LED mode is FIXED (LED_MODE_FIXED)
uint8_t LED_GetColor();                       // Returns the current color

#endif /* __LED_H */
