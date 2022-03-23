#ifndef __RTCC_H
#define __RTCC_H

#include "stm32f0xx.h"
#include "time.h"

#define RTC_CLOCK_SOURCE_LSI     //Low Speed Internal Oscillator
//#define RTC_CLOCK_SOURCE_LSE   //Low Speed External Oscillator

typedef struct
{
	uint8_t WeekDay;/* This parameter can be a value as following:
							@arg RTC_Weekday_Sunday		-->> (uint8_t)0x01
							@arg RTC_Weekday_Monday		-->> (uint8_t)0x02
							@arg RTC_Weekday_Tuesday	-->> (uint8_t)0x03
							@arg RTC_Weekday_Wednesday	-->> (uint8_t)0x04
							@arg RTC_Weekday_Thursday	-->> (uint8_t)0x05
							@arg RTC_Weekday_Friday		-->> (uint8_t)0x06
							@arg RTC_Weekday_Saturday	-->> (uint8_t)0x07	*/

	uint8_t Date;	/* This parameter must be set to a value in the 1-31 range. */

	uint8_t Month;	/* This parameter can be a value as following:
							@arg RTC_Month_January	-->> (uint8_t)0x01
							@arg RTC_Month_February	-->> (uint8_t)0x02
							@arg RTC_Month_March	-->> (uint8_t)0x03
							@arg RTC_Month_April	-->> (uint8_t)0x04
							@arg RTC_Month_May		-->> (uint8_t)0x05
							@arg RTC_Month_June		-->> (uint8_t)0x06
							@arg RTC_Month_July		-->> (uint8_t)0x07
							@arg RTC_Month_August	-->> (uint8_t)0x08
							@arg RTC_Month_September-->> (uint8_t)0x09
							@arg RTC_Month_October	-->> (uint8_t)0x10
							@arg RTC_Month_November	-->> (uint8_t)0x11
							@arg RTC_Month_December -->> (uint8_t)0x12	*/

	uint8_t Year;	/* This parameter must be set to a value in the 0-99 range. */

	uint8_t Hours;	/* This parameter must be set to a value in the 0-12 range
                        if the RTC_HourFormat_12 is selected or 0-23 range if
                        the RTC_HourFormat_24 is selected. */

	uint8_t Minutes;/* This parameter must be set to a value in the 0-59 range. */

	uint8_t Seconds;/* This parameter must be set to a value in the 0-59 range. */
}RTCC_TimeDateTypeDef;

//-------------------------------
// Functions Definitions
void RTCC_Init (void);
void RTCC_SetTimeDate(RTCC_TimeDateTypeDef RTCC_TimeDate);
void RTCC_GetTimeDate(RTCC_TimeDateTypeDef* RTCC_TimeDate);

void RTCC_StartCount(void);

#endif /* __RTCC_H */
