#include "PWM.h"

#include "stm32f0xx.h"                  // STM32F0xx Definitions

//-------------------------------
// Ports and Pins Definitions
#define PWM_PORT		    GPIOA
#define PWM_PIN_LED_RED		GPIO_Pin_8   // PA8  - TIM1_CH1
#define PWM_PIN_LED_GREEN	GPIO_Pin_10  // PA10 - TIM1_CH3
#define PWM_PIN_LED_BLUE	GPIO_Pin_9   // PA9  - TIM1_CH2
#define PWM_PIN_BUZZER      GPIO_Pin_11  // PA11 - TIM1_CH4
#define PWM_PIN_TOUCH       GPIO_Pin_6   // PA6 -  TIM16_CH?

#define PWM_PIN_SOURCE_LED_RED    GPIO_PinSource8
#define PWM_PIN_SOURCE_LED_GREEN  GPIO_PinSource10
#define PWM_PIN_SOURCE_LED_BLUE   GPIO_PinSource9
#define PWM_PIN_SOURCE_BUZZER     GPIO_PinSource11
#define PWM_PIN_SOURCE_TOUCH      GPIO_PinSource6

//-------------------------------
// TODO: Description
uint16_t TimerPeriod = 0;

//-------------------------------
// TODO: Description
uint8_t PWM_Initialized = 0;

//==================================================================================================
//
//==================================================================================================
void PWM_Init_1 (void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOB Clocks enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* GPIOB Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin   = PWM_PIN_LED_RED | PWM_PIN_LED_GREEN | PWM_PIN_LED_BLUE | PWM_PIN_BUZZER;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(PWM_PORT, PWM_PIN_SOURCE_LED_RED,   GPIO_AF_2);
	GPIO_PinAFConfig(PWM_PORT, PWM_PIN_SOURCE_LED_GREEN, GPIO_AF_2);
	GPIO_PinAFConfig(PWM_PORT, PWM_PIN_SOURCE_LED_BLUE,  GPIO_AF_2);
	GPIO_PinAFConfig(PWM_PORT, PWM_PIN_SOURCE_BUZZER,    GPIO_AF_2);

	/* Compute the value to be set in ARR regiter to generate signal frequency at 2.5Khz */
	TimerPeriod = (SystemCoreClock / 2500 ) - 1;// (48000000/2500)-1 = 19199

	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode 		 = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity 	 = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

//==================================================================================================
//
//==================================================================================================
void PWM_Init_2 (void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	uint16_t TimerPeriod_2 = 0;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA Clocks enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* GPIOA Configuration: */
	GPIO_InitStructure.GPIO_Pin   = PWM_PIN_TOUCH; // PIN A6
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, PWM_PIN_SOURCE_TOUCH, GPIO_AF_5);

	/* Compute the value to be set in ARR regiter to generate signal frequency at 1Khz */
	TimerPeriod_2 = (SystemCoreClock / 1000) - 1;

	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler   = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period 	  = TimerPeriod_2;
	TIM_TimeBaseStructure.TIM_ClockDivision 	= 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode 		 = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM16, &TIM_OCInitStructure);

	/* TIM1 counter enable */
	TIM_Cmd(TIM16, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM16, ENABLE);

	uint32_t Compare = (uint32_t) (((uint32_t) 50 * (TimerPeriod_2 - 1)) / 100);

	TIM_SetCompare1(TIM16, Compare);
}

//==================================================================================================
//
//==================================================================================================
void PWM_Init(void)
{
	if( PWM_Initialized == 0)
	{
		PWM_Init_1();
		PWM_Init_2();

		PWM_Initialized = 1;
	}
}

//==================================================================================================
//
//==================================================================================================
void PWM_SetDutyCycle(uint8_t Channel, uint8_t DutyCycle)
{
	if(PWM_Initialized == 1)
	{
		uint32_t Compare = (uint32_t) (((uint32_t) DutyCycle * (TimerPeriod - 1)) / 100);

		if(Channel == PWM_CHANNEL_LED_RED  ) TIM_SetCompare1(TIM1, Compare);
		if(Channel == PWM_CHANNEL_LED_GREEN) TIM_SetCompare3(TIM1, Compare);
		if(Channel == PWM_CHANNEL_LED_BLUE ) TIM_SetCompare2(TIM1, Compare);
		if(Channel == PWM_CHANNEL_BUZZER   ) TIM_SetCompare4(TIM1, Compare);
	}
}
