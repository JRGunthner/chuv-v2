#include "RTCC.h"
#include "time.h"

__IO uint32_t AsynchPrediv = 0;
__IO uint32_t SynchPrediv = 0;

//==================================================================================================
//
//==================================================================================================
void RTCC_Config(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
	/* The RTC Clock may varies due to LSI frequency dispersion. */
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);

	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSI is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	SynchPrediv  = 0x18F;
	AsynchPrediv = 0x63;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	SynchPrediv  = 0xFF;
	AsynchPrediv = 0x7F;

#else
	#error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronization */
	RTC_WaitForSynchro();
}

//==================================================================================================
//
//==================================================================================================
void RTCC_TimeRegulate(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStruct;

	RTC_TimeStructure.RTC_H12  = RTC_H12_AM;
	RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Monday;
	RTC_DateStruct.RTC_Month   = RTC_Month_January;
	RTC_DateStruct.RTC_Date    = 1;
	RTC_DateStruct.RTC_Year    = 15;

	/* Configure the RTC time register */
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct) == ERROR)
	{
		// TODO - Tratar o retorno
		//printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
	}

	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 8;
	RTC_TimeStructure.RTC_Minutes = 0;
	RTC_TimeStructure.RTC_Seconds = 0;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		// TODO - Tratar o retorno
		//printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	}
}

//==================================================================================================
//
//==================================================================================================
void RTCC_Init (void)
{
	RTC_InitTypeDef RTC_InitStructure;

	/* RTC configuration  */
	RTCC_Config();

	/* Configure the RTC data register and RTC prescaler */
	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv  = SynchPrediv;
	RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;

	/* Check on RTC init */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		// TODO - Tratar o retorno
		//printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
	}

	RTCC_TimeRegulate();

}

//==================================================================================================
//
//==================================================================================================
void RTCC_SetTimeDate(RTCC_TimeDateTypeDef RTCC_TimeDate)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStruct;

	RTC_DateStruct.RTC_WeekDay = RTCC_TimeDate.WeekDay;
	RTC_DateStruct.RTC_Month   = RTCC_TimeDate.Month;
	RTC_DateStruct.RTC_Date    = RTCC_TimeDate.Date;
	RTC_DateStruct.RTC_Year    = RTCC_TimeDate.Year;

	/* Configure the RTC time register */
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct) == ERROR)
	{
		// TODO - Tratar o retorno
		//printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
	}

	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = RTCC_TimeDate.Hours;
	RTC_TimeStructure.RTC_Minutes = RTCC_TimeDate.Minutes;
	RTC_TimeStructure.RTC_Seconds = RTCC_TimeDate.Seconds;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		// TODO - Tratar o retorno
		//printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	}
}

//==================================================================================================
//
//==================================================================================================
void RTCC_GetTimeDate(RTCC_TimeDateTypeDef* RTCC_TimeDate)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStruct;

	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

	RTCC_TimeDate->WeekDay = RTC_DateStruct.RTC_WeekDay;
	RTCC_TimeDate->Date    = RTC_DateStruct.RTC_Date;
	RTCC_TimeDate->Month   = RTC_DateStruct.RTC_Month;
	RTCC_TimeDate->Year    = RTC_DateStruct.RTC_Year;

	RTCC_TimeDate->Hours   = RTC_TimeStructure.RTC_Hours;
	RTCC_TimeDate->Minutes = RTC_TimeStructure.RTC_Minutes;
	RTCC_TimeDate->Seconds = RTC_TimeStructure.RTC_Seconds;
}

void RTCC_SecondsCounter(void)
{

}

