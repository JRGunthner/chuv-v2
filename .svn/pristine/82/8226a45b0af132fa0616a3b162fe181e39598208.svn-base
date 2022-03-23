#include "NTC.h"
#include "AD.h"

//----------------------------------------------
// Errors
uint8_t NTCError = NTC_OK;

//==================================================================================================
//
//==================================================================================================
void NTC_Init (void)
{
	AD_Init();
}

//==================================================================================================
//
//==================================================================================================
void NTC_Error()
{
	NTCError = NTC_OK;

	// Inlet temperature reading fail - if value is out of (5° ate 95°) range
	if((AD_GetValue(AD_NTC1)>950) || (AD_GetValue(AD_NTC1)<50))
	{
		NTCError = NTC_ERROR_INLET_TEMPERATURE;
	}

	// Outlet temperature reading fail - if value is out of (5° ate 95°) range
	if((AD_GetValue(AD_NTC2)>950) || (AD_GetValue(AD_NTC2)<50))
	{
		NTCError = NTC_ERROR_OUTLET_TEMPERATURE;
	}

	// Resistance temperature reading fail - if value is out of (5° ate 95°) range
	if((AD_GetValue(AD_NTC3)>950) || (AD_GetValue(AD_NTC3)<50))
	{
		NTCError = NTC_ERROR_RESISTANCE_TEMPERATURE;
	}

	// Inlet sensor if (TE > TS) e (TE > TA)
	if((AD_GetValue(AD_NTC1)<AD_GetValue(AD_NTC2)) && (AD_GetValue(AD_NTC1)<AD_GetValue(AD_NTC3)))
	{
		NTCError = NTC_ERROR_INLET_SENSOR;
	}

	// Outlet sensor if (TS < TE)
	if(AD_GetValue(AD_NTC2)<AD_GetValue(AD_NTC1))
	{
		NTCError = NTC_ERROR_OUTLET_SENSOR;
	}

	// Resistance sensor if (TA < TE)
	if(AD_GetValue(AD_NTC3)<AD_GetValue(AD_NTC1))
	{
		NTCError = NTC_ERROR_RESISTANCE_SENSOR;
	}
}

//==================================================================================================
//
//==================================================================================================
int16_t NTC_GetWaterTemperature(uint8_t Type)
{
	uint16_t AD_Value = 0;

	if(Type == TEMPERATURE_INLET)
	{
		AD_Value = AD_GetValue(AD_NTC1);
	}
	else if(Type == TEMPERATURE_OUTLET)
	{
		AD_Value = AD_GetValue(AD_NTC2);
	}
	else if(Type == TEMPERATURE_RESISTANCE)
	{
		AD_Value = AD_GetValue(AD_NTC3);
	}

	 NTC_Error();

    /***********************************/
    /*                                 */
	/*   +-------------+-----------+   */
	/*   | Temperature |  Average  |   */
    /*   |   (XX.X)    | AD_value  |   */
	/*   +-------------+-----------+   */
	/*   |     126     |   1224    |   */
	/*   |     263     |   1891    |   */
	/*   +-------------+-----------+   */
    /*                                 */
    /***********************************/

	return (int16_t)((AD_Value*0.206)-126);
}

//==================================================================================================
// Get TRIAC Error
//==================================================================================================
uint8_t NTC_GetError()
{
	return NTCError;
}
