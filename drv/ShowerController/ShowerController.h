#ifndef __SHOWER_CONTROLLER_H
#define __SHOWER_CONTROLLER_H

#include "stm32f0xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//-------------------------------
// ShowerController_SetShowerState() Defines
#define SHOWER_TIME_IDLE	 0x01
#define SHOWER_TIME_STARTED	 0x02
#define SHOWER_TIME_FINISHED 0x03

//-------------------------------
// Functions Definitions
void ShowerController_RemoteControl(void);
void ShowerController_Init(void);
void ShowerController_Task(void);

void ShowerController_SetShowerState(uint8_t ShowerState);
uint8_t ShowerController_GetShowerState(void);

#endif /* __SHOWER_CONTROLLER_H */
