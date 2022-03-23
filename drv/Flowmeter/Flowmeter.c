#include "Flowmeter.h"

#include "stm32f0xx.h"
#include "stdio.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "timers.h"

#include "main.h"

#define N_FLOW 9

uint32_t Frequency;

uint16_t Flow;
int16_t  FlowDerivative;

void * pvTimerID;
TimerHandle_t xFlowTimer;

//==================================================================================================
//
//==================================================================================================
void vFlowTimerCallback(TimerHandle_t pxTimer)
{
	Frequency = 0;
	Flow = 0;
	FlowDerivative = 0;

	xTimerStop(pxTimer, 0);
}

//==================================================================================================
//
//==================================================================================================
void Flowmeter_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* TIM15 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);

	/* GPIOB clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* TIM15 chennel2 configuration : PA.01 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15; // Flowmeter PIN
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect TIM pin to AF2 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_1);

	/* Enable the TIM15 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */

	TIM_TimeBaseStructure.TIM_Prescaler 	= (SystemCoreClock / 1000) - 1; // (48000000/1000)-1 = 47999
	TIM_TimeBaseStructure.TIM_Period 		= 0xFFFF; // Maximal
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel 	= TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter 	= 0x0;

	TIM_PWMIConfig(TIM15, &TIM_ICInitStructure);

	/* Select the TIM15 Input Trigger: TI2FP2 */
	TIM_SelectInputTrigger(TIM15, TIM_TS_TI2FP2);

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(TIM15, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM15,TIM_MasterSlaveMode_Enable);

	/* TIM enable counter */
	TIM_Cmd(TIM15, ENABLE);

	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(TIM15, TIM_IT_CC2, ENABLE);

	Frequency = 0;
	Flow = 0;
	FlowDerivative = 0;

	xFlowTimer = xTimerCreate("FlowTimer",
							  100 / portTICK_PERIOD_MS,
							  pdFALSE,
							  pvTimerID,
							  vFlowTimerCallback);

	xTimerStopFromISR(xFlowTimer, 0);

}

//==================================================================================================
//
//==================================================================================================
void Flowmeter_FlowConversion()
{
	if(Frequency > 0)
	{
		/******************************/
		/*                            */
		/*   +--------+-----------+   */
		/*   | Liters | Frequency |   */
		/*   | (X.XX) |           |   */
		/*   +--------+-----------+   */
		/*   |   100  |      7    |   */
		/*   |   200  |     13    |   */
		/*   |   300  |     21    |   */
		/*   |   400  |     28    |   */
		/*   |   500  |     35    |   */
		/*   |   600  |     42    |   */
		/*   |   700  |     50    |   */
		/*   |   800  |     55    |   */
		/*   |   900  |     62    |   */
		/*   +--------+-----------+   */
		/*                            */
		/******************************/

		// TODO: Fazer rotinas para calibracao - por hora esta fixo
		Flow = (14.3368 * Frequency) - 1.3948;
	}
	else Flow = 0;
}

//==================================================================================================
//
//==================================================================================================
void Flowmeter_FlowDerivative()
{
	uint8_t i;
	static int16_t Derivative_Vector[N_FLOW+1];

	if(Flow <  FLOW_ON)
	{
		FlowDerivative = 0;
	}
	else
	{
		for(i=N_FLOW; i>=1; i--)
		{
			Derivative_Vector[i] = Derivative_Vector[i-1];
		}

		Derivative_Vector[0] = Flow * N_FLOW;

		FlowDerivative = 0;

		for(i=N_FLOW; i>=1; i--)
		{
			FlowDerivative += (Derivative_Vector[i-1] - Derivative_Vector[i])/i;
		}

		FlowDerivative /= N_FLOW;
	}
}

//==================================================================================================
// Flowmeter
//==================================================================================================
void TIM15_IRQHandler(void)
{
	xTimerStopFromISR(xFlowTimer, 0);

    if(TIM_GetITStatus(TIM15, TIM_IT_CC2) != RESET)
    {
		/* Clear TIM15 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM15, TIM_IT_CC2);

		if(TIM15->CCR2 != 0)
		{
			Frequency = 1000 / TIM15->CCR2;
		}
		else
		{
			Frequency = 0;
		}
    }

    if(Frequency < 3)
    {
    	Frequency = 0;
    }

    xTimerStartFromISR(xFlowTimer, 0);
}

//==================================================================================================
// Flowmeter Task
//==================================================================================================
void Flowmeter_Task(void)
{
	Flowmeter_FlowConversion();
	Flowmeter_FlowDerivative();
}

static uint16_t ForcedFlow = 0;

//==================================================================================================
//
//==================================================================================================
uint16_t Flowmeter_GetFlow()
{
	if(!ForcedFlow)
	{
	return Flow;
}
	else
	{
		return ForcedFlow;
	}
}
//==================================================================================================
// -->> JUST TO DEBUG <<-- FUNCTION THAT FORCES THE FLOW VALUE
//==================================================================================================
void Flowmeter_ForceGetFlow(uint16_t Forced)
{
	ForcedFlow = Forced;
}

//==================================================================================================
//
//==================================================================================================
uint32_t Flowmeter_GetFrequency()
{
	return Frequency;
}

//==================================================================================================
//
//==================================================================================================
int16_t Flowmeter_GetFlowDerivative()
{
	return FlowDerivative;
}

//==================================================================================================
//
//==================================================================================================
void Flowmeter_EnableIRQ (void)
{
	NVIC_EnableIRQ (TIM15_IRQn);
}

//==================================================================================================
//
//==================================================================================================
void Flowmeter_DisableIRQ (void)
{
	NVIC_DisableIRQ (TIM15_IRQn);
}
