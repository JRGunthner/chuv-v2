#ifndef __NTC_H
#define __NTC_H

#include "stm32f0xx.h"

//----------------------------------------------
// Defines for uint8_t NTC_GetError();
#define NTC_OK                           0
#define NTC_ERROR_INLET_TEMPERATURE      1    // Inlet temperature reading fail - if value is out of ( 5� ate 95�) range
#define NTC_ERROR_OUTLET_TEMPERATURE     2    // Outlet temperature reading fail - if value is out of ( 5� ate 95�) range
#define NTC_ERROR_RESISTANCE_TEMPERATURE 3    // Resistance temperature reading fail - if value is out of ( 5� ate 95�) range
#define NTC_ERROR_INLET_SENSOR           4    // Inlet sensor if (TE > TS) e (TE > TA)
#define NTC_ERROR_OUTLET_SENSOR          5    // Outlet sensor if (TS < TE)
#define NTC_ERROR_RESISTANCE_SENSOR      6    // Resistance sensor if (TA < TE)

//-------------------------------
// NTC_GetWaterTemperature(uint8_t Type) defines
#define TEMPERATURE_INLET			1   // Inlet Water temperature Sensor (celsius degrees)
#define TEMPERATURE_OUTLET			2   // Outlet Water temperature Sensor (celsius degrees)
#define TEMPERATURE_RESISTANCE		3   // Water temperature Sensor - Resistance (read from resistance element - celsius degrees)

//-------------------------------
// Functions Definitions
void    NTC_Init (void);
int16_t NTC_GetWaterTemperature(uint8_t Type);   // Return example - 400 is 40.0 C - 355 is 35.5 C
uint8_t NTC_GetError();

#endif /* __NTC_H */
