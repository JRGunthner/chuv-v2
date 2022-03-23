#ifndef TOUCH_H_
#define TOUCH_H_

#include "stm32f0xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//-------------------------------
// AD_GetValue() defines
void TOUCH_Init(xQueueHandle *QueueHandler);
void TOUCH_Task();

int32_t TOUCH_GetArithmeticMeanUp();
int32_t TOUCH_GetArithmeticMeanDown();

#endif /* TOUCH_H_ */
