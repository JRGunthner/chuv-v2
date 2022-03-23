#include "IrDA.h"

//----------------------------------------------
// Data Received from IR - It contains Remote Control Key codes
static uint32_t DataReceived;

xQueueHandle *IrDAQueueHandler;

//==================================================================================================
//
//==================================================================================================
void IrDA_Init(xQueueHandle *QueueHandler)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIOB clock - AHB: advanced high-performance bus*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable SYSCFG clock - APB: advanced peripheral bus*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI0 Line to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

	/* Configure EXTI0 line */
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_ClearITPendingBit(EXTI_Line1);

	/* Timer Configuration */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler 	 = 48;    // 48000000 / 48 = 1000000 -->> 1us resolution
	timerInitStructure.TIM_CounterMode 	 = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period 		 = 10000; // Counter max number
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &timerInitStructure);
	TIM_Cmd(TIM6, ENABLE);

	DataReceived = 0;

	IrDA_EnableIRQ();

	IrDAQueueHandler = QueueHandler;
}

//==================================================================================================
//
//==================================================================================================
uint32_t IrDA_IRQHandler()
{
	//============================================//============================================//============================================//
	// 				Start bit                     //				Bit 1                       //				  Bit 0                       //
	//                                            //                                            //                                            //
	//					4.440ms                   //			    1.600ms                     //			      520us                       //
	//			  +----------------+              //			  +---------+                   //	      		  +---+                       //
	//			  |				   |              //			  |		    |                   //	    		  |	  |                       //
	//			  | 			   |              //			  |         |                   //		    	  |   |                       //
	//		------+				   +----          //		------+	        +----               //		    ------+	  +----                   //
	//					  		    600us         //				     	 600us              //                     600us                  //
	//                                            //                                            //                                            //
	//============================================//============================================//============================================//

	static uint32_t RisingEdgeTime;
	static uint32_t FallingEdgeTime;
	static uint32_t ReceivedBitsCount;

	EXTI->PR |= 0x00000002;	// Clear the IRQ pending flag

	if(((EXTI->FTSR & 0x00000002) == 0) && ((EXTI->RTSR & 0x00000002) > 0)) //if RisingEdge
	{
		TIM_SetCounter(TIM6, 0);
		RisingEdgeTime = TIM_GetCounter(TIM6);

		// Configuration for the Next Edge (FallingEdge)
		EXTI->FTSR |= 0x00000002;  // Falling trigger selection register
		EXTI->RTSR &= 0xFFFFBFFF;  // Rising trigger selection register
	}
	else //if FallingEdge
	{
		FallingEdgeTime = TIM_GetCounter(TIM6);

		// Configuration for the Next Edge (RisingEdge)
		EXTI->FTSR &= 0xFFFFFFFD;  // Falling trigger selection register
		EXTI->RTSR |= 0x00000002;  // Rising trigger selection register

		// Shifts the received bit to the left (it opens space for the next bit)
		DataReceived = DataReceived << 1;

		// Start Bit
		if((FallingEdgeTime - RisingEdgeTime) >= 4000) // Start Bit (4.440ms)
		{
			DataReceived = 0;
			ReceivedBitsCount = 0;
		}
		else if((FallingEdgeTime - RisingEdgeTime) >= 1000) // Bit 1 (1.600ms)
		{
			DataReceived |= 0b1;
		}
		//else Bit 0 (520us) (DataReceived is 0 by default, we dont need the else condition)

		// Command ready
		if(++ReceivedBitsCount >= 33) // 32 data bits plus start bit
		{
			ReceivedBitsCount = 0;
			return DataReceived;
		}
	}

	return 0;
}

//==================================================================================================
//
//==================================================================================================
void EXTI0_1_IRQHandler(void)
{
	uint32_t Data;

    static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	IrDA_DisableIRQ(); // Disable the IRQ
	Data = IrDA_IRQHandler();
	IrDA_EnableIRQ();

	if(Data > 0)
	{
		xQueueSendFromISR(*IrDAQueueHandler, &Data, &xHigherPriorityTaskWoken);
	}
}

//==================================================================================================
//
//==================================================================================================
void IrDA_EnableIRQ(void)
{
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

//==================================================================================================
//
//==================================================================================================
void IrDA_DisableIRQ(void)
{
	NVIC_DisableIRQ(EXTI0_1_IRQn);
}
