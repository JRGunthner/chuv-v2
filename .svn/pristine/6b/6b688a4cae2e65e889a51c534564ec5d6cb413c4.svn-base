#include "Touch.h"
#include "AD.h"
#include "PWM.h"

enum {TOUCH_PRESS, TOUCH_RELEASE, TOUCH_EVENT};

uint8_t StateTouch = TOUCH_PRESS;

int32_t LastMeanUp = 0;
int32_t LastMeanDown = 0;

int32_t MeanUp = 0;
int32_t MeanDown = 0;

int32_t DeltaUp = 0;
int32_t DeltaDown = 0;

int32_t LastDeltaUp = 0;
int32_t LastDeltaDown = 0;

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
int32_t TOUCH_GetArithmeticMeanUp (void)
{
	return DeltaUp;
}

//==================================================================================================
//
//==================================================================================================
int32_t TOUCH_GetArithmeticMeanDown (void)
{
	return DeltaDown;
}

//==================================================================================================
//
//==================================================================================================
void TOUCH_Task (void)
{
	uint32_t Data;

	static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	LastMeanUp 	 = MeanUp;
	LastMeanDown = MeanDown;
	//Atualiza último valor da média

	MeanUp 	 = (LastMeanUp 	 + (int32_t)AD_GetValue(AD_TOUCH_UP))  /2;
	MeanDown = (LastMeanDown + (int32_t)AD_GetValue(AD_TOUCH_DOWN))/2;
	//Valor médio entre a última média e o valor atual lido no AD

	LastDeltaUp	  = DeltaUp;
	LastDeltaDown = DeltaDown;
	//Atualiza último valor da diferença

	if((MeanDown - LastMeanDown) > 0)
	{
		DeltaDown = MeanDown - LastMeanDown;
	}
	else
	{
		DeltaDown = LastMeanDown - MeanDown;
	}
	//Diferença entre última medida e medida atual

	if((MeanUp - LastMeanUp) > 0)
	{
		DeltaUp = MeanUp - LastMeanUp;
	}
	else
	{
		DeltaUp = LastMeanUp - MeanUp;
	}
	//Diferença entre última medida e medida atual

	DeltaUp	  = (LastDeltaUp + DeltaUp)/2;
	DeltaDown = (LastDeltaDown + DeltaDown)/2;
	//Media entre a última diferença e a diferença atual

	LastCountUp = CountUp;
	LastCountDown = CountDown;
	//Atualiza último valor dos contadores

	if(DeltaDown > 150)		CountDown++;
	else if(DeltaUp > 150)	CountUp++;

	if(LastCountUp 	 == CountUp)	MaxCountUp++;
	if(LastCountDown == CountDown)	MaxCountDown++;

	if(MaxCountUp > 20)
	{
		MaxCountUp = 0;
		CountUp = 0;
	}

	if(MaxCountDown > 20)
	{
		MaxCountDown = 0;
		CountDown = 0;
	}

	if((CountDown > 3) || (CountUp > 3))
	{
		CountUpRelease = 0;
		CountDownRelease = 0;

		MaxCountUp = 0;
		MaxCountDown = 0;

		if(CountDown > 3)
		{
			Data = 0x24DB09F6; // Equal to D2 in "IrDA.h"
		}
		else if(CountUp > 3)
		{
			Data = 0x24DB51AE; // Equal to B2 in "IrDA.h"
		}

		xQueueSendFromISR(*TouchQueueHandler, &Data, &xHigherPriorityTaskWoken);

		CountDown = 0;
		CountUp   = 0;

		DeltaUp   = 0;
		DeltaDown = 0;

		vTaskDelay(400);
	}
	else
	{
		vTaskDelay(10);
	}
}
