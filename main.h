#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx.h"

// User Shower Mode
#define SHOWER_MODE_TEMPERATURE 0x01
#define SHOWER_MODE_POWER		0x02

// Initial Temperature Limit
#define TEMPERATURE_LIMIT_ON    0x0F
#define TEMPERATURE_LIMIT_OFF   0xF0

// User Temperature SetPoint
#define DEFAULT_SETPOINT  		365
#define MIN_SETPOINT	  		205
#define MAX_SETPOINT	  		455

// Temperature Protection
#define MAX_TEMPERATURE 		499

#endif /* __MAIN_H */
