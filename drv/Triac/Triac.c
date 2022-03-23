#include "Triac.h"
#include <stdbool.h>

//----------------------------------------------
// Debug
//static uint8_t debug = 0;		// Used to check if "Zero Crossing" interruption is working ( alternates 0/1 on pin every time a interruption occur)

//----------------------------------------------
// Variable used to control the amount of power delivered to the resistance element
static uint8_t ShowerPower; 	// Power to be applied. Controlled by TRIAC_SetPower() and TRIAC_GetPower()

//----------------------------------------------
// State machine
static uint32_t LastEdgeTime;	// Time that the last edge occurred
static uint32_t EdgeTime;		// Time that current Edge occurred
static uint8_t PulseState = 1;	// State machine control inside TRIAC_Pulse function

//----------------------------------------------
// Lost edges Error
static uint32_t LostEdgeCounter = 0;// Increments when a lost of edge is detected
#define MAX_LOST_EDGE 100			// Maximum of lost edges to Generates an error

//----------------------------------------------
// Triac Test
static uint32_t TestTriacCount  = 0;// Pulse counter - It counts how many edges to perform triac test function
static bool 	TestTriac 		= false;
#define TRIAC_TEST_COUNT_MAX 	240	// Triac Test Pulses - Code will perform a triac test when TestTriacCount reaches this value (1/60)*pulses=time -> Ex (1/60)*240 = 4s

//----------------------------------------------
// For 60Hz one cycle is 16.67ms
#define EDGE_TIME_MIN 16000			// 16.000 ms
#define EDGE_TIME_MAX 18000			// 18.000 ms
#define HALF_CYCLE 	  8333			// 8.300 ms
#define PULSE_LENGHT  100			// 0.1 ms - controls pulse lenght inside TRIAC_Pulse function

//----------------------------------------------
// Errors
uint8_t TriacError = 0;

//----------------------------------------------
// Table that converts Triac Power in delay for the timer that controls TRIAC_Pulse function
#include "Triac_Table.h"

//==================================================================================================
// TRIAC PINs configuration
//==================================================================================================
static void TRIAC_PinOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOF Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configure PF7 and PF6 in output pushpull mode */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	/* TRIAC 1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);

	/* TRIAC 2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

//==================================================================================================
// Debug PIN configuration
//==================================================================================================
static void TRIAC_PinDebug(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure PA15 in output pushpull mode
	 * In the board the reference is P8
	 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15; //P8 in the board
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

//==================================================================================================
// Protection PIN configuration
//==================================================================================================
static void TRIAC_PinProtection (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

	/* Configure PA15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0; // Protection PIN
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

}

//==================================================================================================
// Zero Crossing PIN configuration
//==================================================================================================
static void TRIAC_PinZC (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIOC clock - AHB: advanced high-performance bus*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Configure PF1 pin as input floating */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8; // Zero Crossing PIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable SYSCFG clock - APB: advanced peripheral bus*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI13 Line to PB8 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);

	/* Configure EXTI13 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	//EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI13 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//==================================================================================================
// TIM 14 is used to Check if the times between edges is right for 60Hz
//==================================================================================================
static void TRIAC_TIM14(void)
{
	TIM_TimeBaseInitTypeDef timerInitStructure;

	/* Timer Configuration */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	timerInitStructure.TIM_Prescaler = 48;    // 48000000 / 48  = 10000  -> 1us
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 0xFFFF;   // Counter max number
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM14, &timerInitStructure);
	TIM_Cmd(TIM14, ENABLE);

	TIM_SetCounter(TIM14, 0);

	LastEdgeTime = 0;
	EdgeTime = 0;
}

//==================================================================================================
// TIM 17 is used for the pulse function
//==================================================================================================
static void TRIAC_TIM17(void)
{
	TIM_TimeBaseInitTypeDef timerInitStructure;
	NVIC_InitTypeDef nvicStructure;

	/* Timer Configuration */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

	timerInitStructure.TIM_Prescaler = 48;		// 48000000 / 48 = 1000000  -> 1us
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = HALF_CYCLE; // Counter max number
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM17, &timerInitStructure);
	TIM_Cmd(TIM17, ENABLE);

	TIM_SetCounter(TIM17, 0);

	TIM_ITConfig(TIM17, TIM_IT_CC1, ENABLE);

	nvicStructure.NVIC_IRQChannel = TIM17_IRQn;
	nvicStructure.NVIC_IRQChannelPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	NVIC_DisableIRQ (TIM17_IRQn);
}

//==================================================================================================
//
//==================================================================================================
void TRIAC_Init(void)
{
	TRIAC_PinProtection();	// Protection PIN configuration
	TRIAC_PinOut();			// TRIAC PINs configuration
	TRIAC_PinZC();			// Zero Crossing PIN configuration

	TRIAC_TIM17();			// Timer used to pulse function
	TRIAC_TIM14();			// Timer used to check the rigth 60Hz

	TRIAC_PinDebug();		// Debug PIN configuration

	TRIAC_EnableIRQ();

	ShowerPower = 0;
}

//==================================================================================================
// Set TRIAC Power
//==================================================================================================
void TRIAC_SetPower(uint8_t Power)
{
	ShowerPower = Power;
}

//==================================================================================================
// Get TRIAC Error
//==================================================================================================
uint8_t TRIAC_GetError()
{
	if(LostEdgeCounter > MAX_LOST_EDGE)
	{
		return TRIAC_ERROR_LOST_EDGES;
	}
	else
	{
		return TriacError;
	}
}

//==================================================================================================
//
//==================================================================================================
void TRIAC_LostEdges()
{
	LostEdgeCounter++;
}

//==================================================================================================
// Get current TRIAC Power
//==================================================================================================
uint8_t TRIAC_GetPower(void)
{
	return ShowerPower;
}

//==================================================================================================
// Checks if times between edges is right for 60Hz (Returns true if time is right and false if not)
//==================================================================================================
static bool TRIAC_Check60Hz(void)
{
	bool Ret = false;
	uint32_t Delta;

	EdgeTime = TIM_GetCounter(TIM14);
	Delta = EdgeTime - LastEdgeTime;

	if(Delta > EDGE_TIME_MIN)
	{
		TIM_SetCounter(TIM14, 0);
		LastEdgeTime = TIM_GetCounter(TIM14);

		// Check if interruption time is right. This is done to avoid a false edge
		if((Delta > EDGE_TIME_MIN) && (Delta < EDGE_TIME_MAX)) Ret = true;
	}

	return Ret;
}

//==================================================================================================
// Short circuit test in TRIACs
//==================================================================================================
static void TRIAC_Test (void)
{
	//----------------------------------
	// Test T1 and T2
	// Desliga T1 e T2 e verifica o estado do pino Prote��o (PF0)

	GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0); // TRIAC 1
	GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0); // TRIAC 2

	if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0) // Pin Protection Triac
	//Reading 0v, short circuit in T1 e T2
	{
		TriacError = TRIAC_ERROR_T3;
	}
	else
	//Reading "220v"
	{
		TriacError = TRIAC_OK;
	}

	//----------------------------------
	// Test T1
	if(TriacError == TRIAC_OK)
	{
		//Desliga T1 e liga T2 e verifica o estado do pino Prote��o (PF0)
		GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0); // TRIAC 1
		GPIO_WriteBit(GPIOC,GPIO_Pin_13, 1); // TRIAC 2

		if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0) // Pin Protection Triac
		//Reading 0v, short circuit in T1
		{
			TriacError = TRIAC_ERROR_T1;
		}
	}

	//----------------------------------
	// Test T2
	if( TriacError == TRIAC_OK )
	{
		//Desliga T2 e liga T1 e verifica o estado do pino Prote��o (PF0)
		GPIO_WriteBit(GPIOB,GPIO_Pin_9,  1); // TRIAC 1
		GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0); // TRIAC 2

		if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) == 0) // Pin Protection Triac
		//Reading 0v, short circuit in T2
		{
			TriacError = TRIAC_ERROR_T2;
		}
	}

	//----------------------------------
	// Turn off TRIACs
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0);
	GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0);
}

//==================================================================================================
//
//==================================================================================================
static void TRIAC_Control(void)
{
	if(TestTriacCount < TRIAC_TEST_COUNT_MAX) // TRIAC_TEST_COUNT_MAX = 240
	{
		TestTriacCount++;
		TestTriac = false;
		TIM_SetCounter(TIM17, TRIAC_POWER_TABLE[ShowerPower]);
		//TIM_ClearITPendingBit(TIM17, TIM_IT_CC1);
		//NVIC_EnableIRQ (TIM17_IRQn);
	}
	else
	{
		TestTriacCount = 0;
		TestTriac = true;
		TIM_SetCounter(TIM17, HALF_CYCLE/2);
		//TIM_ClearITPendingBit(TIM17, TIM_IT_CC1);
		//NVIC_EnableIRQ (TIM17_IRQn);
	}

	TIM_ClearITPendingBit(TIM17, TIM_IT_CC1);
	NVIC_EnableIRQ(TIM17_IRQn);
}

//==================================================================================================
// Called every time Zero Crossing pin gets a rising edge
//==================================================================================================
void EXTI4_15_IRQHandler(void)
{
	static bool debug = 0;	// Used to check if "Zero Crossing" interruption is working ( alternates 0/1 on pin every time a interruption occur)

	if(TRIAC_Check60Hz() == true)
	{
		// For debug purposes
		debug = ~debug;
		GPIO_WriteBit(GPIOA, GPIO_Pin_15, debug);

		if(LostEdgeCounter > 0) LostEdgeCounter--;

		TRIAC_Control();
	}
	else
	{
		LostEdgeCounter++;
	}

	EXTI->PR |= 0x00000002;		// Clear the IRQ pending flag
}

//==================================================================================================
// Shooting of TRIACs
//==================================================================================================
static void TRIAC_Pulse()
{
	switch (PulseState)
	{	
		//---------------------------------------------------
		// First Pulse On
		//	  +--
		//    |
		// ___|
		case 1:
		{
			PulseState = 2;
			// Se pot�ncia acima de 0 e TRIACs n�o est�o em curto, liga T1 e T2.
			// Otherwise, turn then off
			if((ShowerPower > 0) && (TriacError == TRIAC_OK))
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_9,  1); // TRIAC 1
				GPIO_WriteBit(GPIOC,GPIO_Pin_13, 1); // TRIAC 2
			}
			else
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0); // TRIAC 1
				GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0); // TRIAC 2
			}
			TIM_SetCounter(TIM17, HALF_CYCLE - PULSE_LENGHT);//(8.3-0.1)ms
			break;
		}

		//---------------------------------------------------
		// First Pulse Off
		//	  +--+
		//    |  |
		// ___|  |______
		case 2:
		{
			PulseState = 3;
			// Turn TRIACs off
			GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0); // TRIAC 1
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0); // TRIAC 2
			TIM_SetCounter(TIM17, PULSE_LENGHT); // 0.1ms
			break;
		}

		//---------------------------------------------------
		// Second Pulse On
		//	  +--+         +--
		//    |  |         |
		// ___|  |_________|
		case 3:
		{
			PulseState = 4;
			// Se pot�ncia acima de 0 e TRIACs n�o est�o em curto, turn on T1 and T2.
			// Otherwise, turn then off
			if((ShowerPower > 0) && (TriacError == TRIAC_OK))
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_9,  1); // TRIAC 1
				GPIO_WriteBit(GPIOC,GPIO_Pin_13, 1); // TRIAC 2
			}
			else
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0); // TRIAC 1
				GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0); // TRIAC 2
			}
			TIM_SetCounter(TIM17, HALF_CYCLE - PULSE_LENGHT);//(8.3-0.1)ms
			break;
		}
		//---------------------------------------------------
		// Second Pulse Off
		//	  +--+         +--+
		//    |  |         |  |
		// ___|  |_________|  |_____
		case 4:
		{
			PulseState = 1;
			GPIO_WriteBit(GPIOB,GPIO_Pin_9,  0); // TRIAC 1
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, 0); // TRIAC 2
			TIM_SetCounter(TIM17, PULSE_LENGHT); // 0.1ms
			NVIC_DisableIRQ (TIM17_IRQn);
			break;
		}
		//---------------------------------------------------
		default:
		{
			NVIC_DisableIRQ (TIM17_IRQn);
			break;
		}
	}
}

//==================================================================================================
//
//==================================================================================================
void TIM17_IRQHandler()
{
	if(TIM_GetITStatus(TIM17, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM17, TIM_IT_CC1);

		if(TestTriac == false)
		{
			TRIAC_Pulse();
		}
		else // TestTriac = true
		{
			TestTriac = false;
			TRIAC_Test();
		}
	}
}

//==================================================================================================
//
//==================================================================================================
void TRIAC_EnableIRQ (void)
{
	NVIC_EnableIRQ(EXTI0_1_IRQn); // ZC
	NVIC_EnableIRQ(TIM17_IRQn);   // Pulse Timer
}

//==================================================================================================
//
//==================================================================================================
void TRIAC_DisableIRQ(void)
{
	NVIC_DisableIRQ(EXTI0_1_IRQn); // ZC
	NVIC_DisableIRQ(TIM17_IRQn);   // Pulse Timer
}
