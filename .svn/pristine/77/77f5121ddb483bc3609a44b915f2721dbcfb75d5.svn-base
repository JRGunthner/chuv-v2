#include "AD.h"

//----------------------------------------------
// Ports and Pins Definitions
#define AD_PIN_VOLTMETER	GPIO_Pin_0 // PA0 - ADC_IN0
#define AD_PIN_NTC1			GPIO_Pin_1 // PA1 - ADC_IN1
#define AD_PIN_NTC2			GPIO_Pin_4 // PA4 - ADC_IN4
#define AD_PIN_NTC3			GPIO_Pin_5 // PA5 - ADC_IN5
#define AD_PIN_TOUCH_UP		GPIO_Pin_7 // PA7 - ADC_IN7
#define AD_PIN_TOUCH_DOWN	GPIO_Pin_0 // PB0 - ADC_IN8

//----------------------------------------------
// Vector that contains converted data from AD
#define SAMPLES 8
volatile uint16_t RegularConvData_Tab[SAMPLES];

//----------------------------------------------
// ADC1 Address
#define ADC1_DR_Address 0x40012440

//-------------------------------
// TODO: Description
uint8_t AD_Initialized = 0;

//==================================================================================================
// ADC PINs configuration
//==================================================================================================
static void ADC_Config(void)
{
	ADC_InitTypeDef	 ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ADC1 DeInit */
	ADC_DeInit(ADC1);

	/* GPIOA and GPIOB Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Configure ADC Channels as analog input */
	GPIO_InitStructure.GPIO_Pin  = AD_PIN_VOLTMETER | AD_PIN_NTC1 | AD_PIN_NTC2 | AD_PIN_NTC3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = AD_PIN_TOUCH_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure ADC Channels as analog input */
	GPIO_InitStructure.GPIO_Pin  = AD_PIN_TOUCH_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode   = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign 	= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStructure);

	//ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_28_5Cycles); // PA0 - ADC_IN0 - VOLTMETER
	ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles);// PA0 - ADC_IN0 - VOLTMETER
	ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_28_5Cycles); // PA1 - ADC_IN1 - NTC1
	ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_28_5Cycles); // PA4 - ADC_IN4 - NTC2
	ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_28_5Cycles); // PA5 - ADC_IN5 - NTC3
	ADC_ChannelConfig(ADC1, ADC_Channel_7, ADC_SampleTime_28_5Cycles); // PB7 - ADC_IN7 - TOUCH_UP
	ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_28_5Cycles); // PB0 - ADC_IN8 - TOUCH_DOWN


	/* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */
	ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_28_5Cycles);

	ADC_TempSensorCmd(ENABLE);

	/* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */
	//ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint, ADC_SampleTime_28_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint, ADC_SampleTime_239_5Cycles);
	ADC_VrefintCmd(ENABLE);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);

	/* ADC DMA request in circular mode */
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	/* Enable ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable the ADC peripheral */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

	/* ADC1 regular Software Start Conv */
	ADC_StartOfConversion(ADC1);
}

//==================================================================================================
//
//==================================================================================================
static void DMA_Config(void)
{
	DMA_InitTypeDef   DMA_InitStructure;

	/* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* DMA1 Channel1 Config */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr 	 = (uint32_t)RegularConvData_Tab;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize 	= SAMPLES;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 	= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize 	 = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode 		= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority 	= DMA_Priority_High;
	DMA_InitStructure.DMA_M2M 		= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
  
}

//==================================================================================================
//
//==================================================================================================
void AD_Init(void)
{
	if(AD_Initialized == 0)
	{
		ADC_Config();
		DMA_Config();

		AD_Initialized = 1;
	}
}

//==================================================================================================
//
//==================================================================================================
uint16_t AD_GetValue(uint8_t index)
{
	if(AD_Initialized == 1)
	{
		//--------------------------------------------------------
		// RegularConvData_Tab[0] - Voltmeter          - ADC_IN0
		// RegularConvData_Tab[1] - NTC1               - ADC_IN1
		// RegularConvData_Tab[2] - NTC2               - ADC_IN4
		// RegularConvData_Tab[3] - NTC3               - ADC_IN5
		// RegularConvData_Tab[4] - Touch +            - ADC_IN7
		// RegularConvData_Tab[5] - Touch -            - ADC_IN8
		// RegularConvData_Tab[6] - Temperature Sensor - ADC_IN16
		// RegularConvData_Tab[7] - Vref               - ADC_IN17
		//--------------------------------------------------------

		/* Test DMA1 TC flag */

		while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET);

		/* Clear DMA TC flag */
		DMA_ClearFlag(DMA1_FLAG_TC1);

		return RegularConvData_Tab[index];
	}
	else
	{
		return 0;
	}
}
