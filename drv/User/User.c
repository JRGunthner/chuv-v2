#include "User.h"
#include "main.h"

static uint8_t UserIndex = 0; // Index of the current user
static User_TypeDef Shower_User[6]; // User settings. User[0] is U6, User[1] is U1, ... , User[5] is U5

//-------------------------------
// Private functions
void User_Copy(uint8_t* Copy, const uint8_t* Original);

//-------------------------------
// Public functions

//------------------------------------
// Get and Set user Shower Mode
uint8_t User_GetShowerMode(uint8_t Index)
{
	return Shower_User[Index].ShowerMode;
}
void User_SetShowerMode(uint8_t ShowerMode, uint8_t Index)
{
	Shower_User[Index].ShowerMode = ShowerMode;
}

//------------------------------------
// Get and Set user LED Mode
uint8_t User_GetLedMode(uint8_t Index)
{
	return Shower_User[Index].LedMode;
}
void User_SetLedMode(uint8_t LedMode, uint8_t Index)
{
	Shower_User[Index].LedMode = LedMode;
}

//------------------------------------
// Get and Set user Led Color
uint8_t User_GetLedColor(uint8_t Index)
{
	return Shower_User[Index].LedColor;
}
void User_SetLedColor(uint8_t LedColor, uint8_t Index)
{
	Shower_User[Index].LedColor = LedColor;
}

//------------------------------------
// Get and Set user Temperature Set Point
uint16_t User_GetTemperatureSetPoint(uint8_t Index)
{
	return Shower_User[Index].TemperatureSetPoint;
}
void User_SetTemperatureSetPoint(uint16_t TemperatureSetPoint, uint8_t Index)
{
	Shower_User[Index].TemperatureSetPoint = TemperatureSetPoint;
}

//------------------------------------
// Get and Set user Power Set Point
uint8_t User_GetPowerSetPoint(uint8_t Index)
{
	return Shower_User[Index].PowerSetPoint;
}
void User_SetPowerSetPoint(uint8_t PowerSetPoint, uint8_t Index)
{
	Shower_User[Index].PowerSetPoint = PowerSetPoint;
}

//------------------------------------
// Get and Set user Temperature Limit
uint8_t User_GetMaxTemperatureLimit(uint8_t Index)
{
	return Shower_User[Index].MaxTemperatureLimit;
}
void User_SetMaxTemperatureLimit(uint8_t MaxTemperatureLimit, uint8_t Index)
{
	Shower_User[Index].MaxTemperatureLimit = MaxTemperatureLimit;
}

//------------------------------------
// Get and Set user Buzzer
uint8_t User_GetBuzzer(uint8_t Index)
{
	return Shower_User[Index].Buzzer;
}
void User_SetBuzzer(uint8_t Buzzer, uint8_t Index)
{
	Shower_User[Index].Buzzer = Buzzer;
}

//------------------------------------
// Get and Set user index
uint8_t User_GetIndex(void)
{
	return UserIndex;
}
void User_SetIndex(uint8_t Index)
{
	UserIndex = Index;
}

//------------------------------------
// Get all user parameters
void User_GetParameters(const User_TypeDef* User_Parameters, uint8_t Index)
{
	User_Copy( (uint8_t*) &Shower_User[Index], (uint8_t*) &User_Parameters );

/*	User_Parameters -> ShowerMode			= Shower_User[Index].ShowerMode;
	User_Parameters -> LedMode				= Shower_User[Index].LedMode;
	User_Parameters -> LedColor				= Shower_User[Index].LedColor;
	User_Parameters -> TemperatureSetPoint	= Shower_User[Index].TemperatureSetPoint;
	User_Parameters -> PowerSetPoint		= Shower_User[Index].PowerSetPoint;
	User_Parameters -> MaxTemperatureLimit	= Shower_User[Index].MaxTemperatureLimit;
	User_Parameters -> Buzzer				= Shower_User[Index].Buzzer;*/
}

void User_Copy(uint8_t* Copy, const uint8_t* Original)
{
	uint8_t auxSz = sizeof(User_TypeDef);
	uint8_t k;

	for(k = 0; k < auxSz; k++)
	{
		Copy[k] = Original[k];
	}
}

void User_SetParameters(User_TypeDef User_Parameters, uint8_t Index)
{
	Shower_User[Index].ShowerMode			= User_Parameters.ShowerMode;
	Shower_User[Index].LedMode				= User_Parameters.LedMode;
	Shower_User[Index].LedColor				= User_Parameters.LedColor;
	Shower_User[Index].TemperatureSetPoint	= User_Parameters.TemperatureSetPoint;
	Shower_User[Index].PowerSetPoint		= User_Parameters.PowerSetPoint;
	Shower_User[Index].MaxTemperatureLimit	= User_Parameters.MaxTemperatureLimit;
	Shower_User[Index].Buzzer				= User_Parameters.Buzzer;
}

void User_SetAllShowerVariables(User_TypeDef User_Parameters)
{
	uint8_t Index;

	UserIndex = 1;  // User 1 is the initial user (Default User)

	for(Index = 0; Index < 6; Index++)
	{
		Shower_User[Index].ShowerMode			= User_Parameters.ShowerMode;
		Shower_User[Index].LedMode				= User_Parameters.LedMode;
		Shower_User[Index].LedColor				= User_Parameters.LedColor;
		Shower_User[Index].TemperatureSetPoint	= User_Parameters.TemperatureSetPoint;
		Shower_User[Index].PowerSetPoint		= User_Parameters.PowerSetPoint;
		Shower_User[Index].MaxTemperatureLimit	= User_Parameters.MaxTemperatureLimit;
		Shower_User[Index].Buzzer				= User_Parameters.Buzzer;
	}
}

void User_ResetShowerVariables(void)
{
	uint8_t Index;

	UserIndex = 1;  // User 1 is the initial user (Default User)

	for(Index = 0; Index < 6; Index++)
	{
		Shower_User[Index].ShowerMode			= 0x01;	// SHOWER_MODE_TEMPERATURE
		Shower_User[Index].LedMode				= 0x04;	// LED_AUTO
		Shower_User[Index].LedColor				= 0x01;	// LED_COLOR_WHITE
		Shower_User[Index].TemperatureSetPoint	= 365;	// DEFAULT_SETPOINT
		Shower_User[Index].PowerSetPoint		= 0;	//
		Shower_User[Index].Buzzer				= 0x0f;	// BUZZER_ENABLED
		Shower_User[Index].MaxTemperatureLimit	= 0x0f;	// TEMPERATURE_LIMIT_ON
	}
}

