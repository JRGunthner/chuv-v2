#ifndef __USER_H
#define __USER_H

#include "stm32f0xx.h"

typedef struct
{
	uint8_t  ShowerMode;			// Shower Mode Power/Temperature
	uint8_t  LedMode;			    // Led Mode Off/Auto/Fixed
	uint8_t  LedColor;			    // Fixed Led color according to the list in LED.h
	uint16_t TemperatureSetPoint;	// User temperature
	uint8_t  PowerSetPoint;		    // User power
	uint8_t  MaxTemperatureLimit; 	// Initial temperature limit on/off
	uint8_t  Buzzer;				// Buzzer on/off
} User_TypeDef;

//-------------------------------
// Functions Definitions

void User_GetParameters(const User_TypeDef* User_Parameters, uint8_t Index);
void User_SetParameters(User_TypeDef  User_Parameters, uint8_t Index);

void User_SetAllShowerVariables(User_TypeDef User_Parameters);
void User_ResetShowerVariables(void);

//------------------------------------
// Get and Set user index
uint8_t User_GetIndex(void);
void	User_SetIndex(uint8_t Index);

//------------------------------------
// Get and Set user Shower Mode
uint8_t User_GetShowerMode(uint8_t Index);
void	User_SetShowerMode(uint8_t ShowerMode, uint8_t Index);

//------------------------------------
// Get and Set user LED Mode
uint8_t User_GetLedMode(uint8_t Index);
void 	User_SetLedMode(uint8_t LedMode, uint8_t Index);

//------------------------------------
// Get and Set user Led Color
uint8_t User_GetLedColor(uint8_t Index);
void	User_SetLedColor(uint8_t LedColor, uint8_t Index);

//------------------------------------
// Get and Set user Temperature Set Point
uint16_t User_GetTemperatureSetPoint(uint8_t Index);
void	 User_SetTemperatureSetPoint(uint16_t TemperatureSetPoint, uint8_t Index);

//------------------------------------
// Get and Set user Power Set Point
uint8_t User_GetPowerSetPoint(uint8_t Index);
void	User_SetPowerSetPoint(uint8_t PowerSetPoint, uint8_t Index);

//------------------------------------
// Get and Set user Temperature Limit
uint8_t User_GetMaxTemperatureLimit(uint8_t Index);
void	User_SetMaxTemperatureLimit(uint8_t MaxTemperatureLimit, uint8_t Index);

//------------------------------------
// Get and Set user Buzzer
uint8_t User_GetBuzzer(uint8_t Index);
void	User_SetBuzzer(uint8_t TemperatureSetPoint, uint8_t Index);

#endif // __USER_H
