#include "Buzzer.h"
#include "PWM.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "stm32f0xx.h"// STM32F0xx Definitions

#define BUZZER_DUTYCYCLE 25

uint8_t BuzzerTime = 0;
uint8_t BuzzerRepeat = 0;

uint8_t BuzzerControl = BUZZER_ENABLED;

SemaphoreHandle_t xBuzzerSemaphore = NULL;

//==================================================================================================
//
//==================================================================================================
void Buzzer_Task()
{
	xSemaphoreTake(xBuzzerSemaphore, portMAX_DELAY);

	uint8_t Repeat = BuzzerRepeat;

	do
	{
		PWM_SetDutyCycle(PWM_CHANNEL_BUZZER, BUZZER_DUTYCYCLE);
		vTaskDelay(BuzzerTime);
		PWM_SetDutyCycle(PWM_CHANNEL_BUZZER, 0);

		if(Repeat > 0) vTaskDelay(30);

	}
	while((--Repeat) > 0);
}

//==================================================================================================
//
//==================================================================================================
void Buzzer_Init (void)
{
	PWM_Init();

	vSemaphoreCreateBinary(xBuzzerSemaphore ); // Create the semaphore
	xSemaphoreTake(xBuzzerSemaphore, 0);       // Take semaphore after creating it.
}

//==================================================================================================
//
//==================================================================================================
void Buzzer_Beep(uint16_t Time, uint8_t Repeat)
{
	BuzzerTime 	 = Time;
	BuzzerRepeat = Repeat;

	if(BuzzerControl == BUZZER_ENABLED)
	{
		xSemaphoreGive(xBuzzerSemaphore);
	}
}

//==================================================================================================
//
//==================================================================================================
void Buzzer_Control(uint8_t EnDis)
{
	BuzzerControl = EnDis;
}
