#include "Touch.h"
#include "AD.h"
#include "PWM.h"

#define TOUCH_BUFFER_LEN 10

uint16_t Buffer_Touch_Up[TOUCH_BUFFER_LEN];
uint16_t Buffer_Touch_Down[TOUCH_BUFFER_LEN];

enum {TOUCH_PRESS, TOUCH_RELEASE, TOUCH_EVENT };

uint8_t StateTouch = TOUCH_PRESS;

uint32_t LastMeanUp = 0;
uint32_t LastMeanDown = 0;

uint32_t MeanUp = 0;
uint32_t MeanDown = 0;

uint8_t CountUp = 0;
uint8_t CountDown = 0;

uint8_t CountUpRelease = 0;
uint8_t CountDownRelease = 0;

uint8_t LastCountUp = 0;
uint8_t LastCountDown = 0;

uint8_t MaxCountUp = 0;
uint8_t MaxCountDown = 0;

xQueueHandle *TouchQueueHandler;

//==================================================================================================
//
//==================================================================================================
void TOUCH_Init (xQueueHandle *QueueHandler)
{
	PWM_Init();
	AD_Init();

	TouchQueueHandler = QueueHandler;
}

//==================================================================================================
//
//==================================================================================================
uint32_t TOUCH_GetArithmeticMeanUp (void)
{
	return MeanUp;
}

//==================================================================================================
//
//==================================================================================================
uint32_t TOUCH_GetArithmeticMeanDown (void)
{
	return MeanDown;
}

//==================================================================================================
//
//==================================================================================================
void TOUCH_Task (void)
{
	uint32_t Data;
	static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	uint8_t j;
	uint32_t MeanUpAux = 0;
	uint32_t MeanDownAux = 0;

	//-----------------------------------------------------
	// Add new value to buffer
	for( j=1; j<TOUCH_BUFFER_LEN; j++)
	{
		Buffer_Touch_Up[j] = Buffer_Touch_Up[j-1];
		Buffer_Touch_Down[j] = Buffer_Touch_Down[j-1];
	}

	Buffer_Touch_Up[0]   = AD_GetValue(AD_TOUCH_UP);
	Buffer_Touch_Down[0] = AD_GetValue(AD_TOUCH_DOWN);

	//-----------------------------------------------------
	// Arithmetic Mean
	for(j=0; j<TOUCH_BUFFER_LEN; j++ )
	{
		MeanUpAux = MeanUpAux + Buffer_Touch_Up[j];
		MeanDownAux = MeanDownAux + Buffer_Touch_Down[j];
	}

	LastMeanUp = MeanUp;
	LastMeanDown = MeanDown;

	MeanUp = MeanUpAux/TOUCH_BUFFER_LEN;
	MeanDown = MeanDownAux/TOUCH_BUFFER_LEN;

	switch (StateTouch)
	{
		//-------------------------------------------------------
		case TOUCH_PRESS:
		{
			LastCountUp = CountUp;
			LastCountDown = CountDown;

			if(  (int32_t)(MeanDown - LastMeanDown) > 600  )
			{
				CountDown++;
			}
			else if(  (int32_t)(MeanUp   - LastMeanUp  ) > 700  )
			{
				CountUp++;
			}

			if( LastCountUp == CountUp )
			{
				MaxCountUp++;
			}
			if( LastCountDown == CountDown )
			{
				MaxCountDown++;
			}

			if( MaxCountUp > 20 )
			{
				MaxCountUp = 0;
				CountUp = 0;
			}
			if( MaxCountDown > 20 )
			{
				MaxCountDown = 0;
				CountDown = 0;
			}

			if( (CountDown > 5) || (CountUp > 5) )
			{
				CountUpRelease = 0;
				CountDownRelease = 0;

				MaxCountUp = 0;
				MaxCountDown = 0;

				StateTouch = TOUCH_RELEASE;
			}

			break;
		}
		//-------------------------------------------------------
		case TOUCH_RELEASE:
		{
			LastCountUp = MaxCountUp;
			LastCountDown = CountDownRelease;

			if(  (int32_t)(MeanDown - LastMeanDown) < 400  )
			{
				CountDownRelease++;
			}
			else if(  (int32_t)(MeanUp   - LastMeanUp  ) < 500  )
			{
				CountUpRelease++;
			}

			if( LastCountUp == CountUpRelease )
			{
				MaxCountUp++;
			}
			if( LastCountDown == CountDownRelease )
			{
				MaxCountDown++;
			}

			if( ( MaxCountUp > 100 ) ||  ( MaxCountDown > 100 ) )
			{
				CountUp = 0;
				CountDown = 0;

				CountUpRelease = 0;
				CountDownRelease = 0;

				MaxCountUp = 0;
				MaxCountDown = 0;

				StateTouch = TOUCH_PRESS;
			}

			if( (CountDownRelease > 10) || (CountUpRelease > 10) )
			{
				StateTouch = TOUCH_EVENT;
			}

			break;
		}
		//-------------------------------------------------------
		case TOUCH_EVENT:
		{
			Data = 0;

			if (CountDown > 3)
			{
				Data = 0x24DB09F6;
			}
			else if (CountUp > 3)
			{
				Data = 0x24DB51AE;
			}

			xQueueSendFromISR(*TouchQueueHandler, &Data, &xHigherPriorityTaskWoken);

			CountUp = 0;
			CountDown = 0;

			CountUpRelease = 0;
			CountDownRelease = 0;

			MaxCountUp = 0;
			MaxCountDown = 0;

			vTaskDelay(400);

			StateTouch = TOUCH_PRESS;

			break;
		}
		//-------------------------------------------------------
		default:
		{
			CountDown = 0;
			CountDownRelease = 0;
			CountUp = 0;
			CountUpRelease = 0;
			break;
		}
	}

	vTaskDelay(10);
}
