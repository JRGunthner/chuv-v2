#include "LED.h"

#include "PWM.h"
#include "NTC.h"
#include "Triac.h"

static uint8_t col = LED_COLOR_WHITE;
static uint16_t LedCount = 0;
static uint16_t LedCountDis = 0;
static uint8_t CurrentColor;
static uint8_t ColorFixed;
static uint8_t LedMode = 0;
static uint8_t gRed;
static uint8_t gGreen;
static uint8_t gBlue;
static uint8_t Enable;

static void LED_SetRGB(uint8_t Red, uint8_t Green, uint8_t Blue);
static void LED_SetColor(uint8_t Led_Color);
static void LED_SetColorByPower(uint8_t Power);
static void LED_SetColorByTemperature(int16_t Temperature);
static uint8_t LED_ReturnColorByPower(uint8_t Power);
static uint8_t LED_ReturnColorByTemperature(int16_t Temperature);

//==================================================================================================
//
//==================================================================================================
void LED_Init(void)
{
	PWM_Init();

	gRed   = 0;
	gGreen = 0;
	gBlue  = 0;

	Enable = LED_ENABLED;
}

//==================================================================================================
//
//==================================================================================================
void LED_Control(uint8_t EnaDis)
{
	Enable = EnaDis;

	if(Enable == LED_DISABLE)
	{
		LedCountDis = LED_TIME_TO_DISABLE;
	}
}

//====================================================================================================
//
//====================================================================================================
void LED_Task ()
{
	if(Enable == LED_DISABLE)
	{
		if(LedCountDis > 0) LedCountDis--;

		if(LedCountDis <= 0)
		{
			LED_SetColor(LED_COLOR_OFF);
		}
	}
	else
	{
		switch(LedMode)
		{
			//--------------------------------------------
			case LED_OFF:
			{
				LED_SetColor(LED_COLOR_OFF);
				break;
			}
			//--------------------------------------------
			case LED_TEMPERATURE:
			{
				LED_SetColorByTemperature(NTC_GetWaterTemperature(TEMPERATURE_OUTLET));
				break;
			}
			//--------------------------------------------
			case LED_POWER:
			{
				LED_SetColorByPower(TRIAC_GetPower());
				break;
			}
			//--------------------------------------------
			case LED_AUTO:
			{
				// Change Color each Ex: 250*10ms = 2,5s
				LedCount++;

				if(LedCount > LED_TIME_TO_CHANGE)
				{
					col++;
					if(col > LED_COLOR_PINK) col = LED_COLOR_WHITE;
					LedCount = 0;
				}

				LED_SetColor(col);
				break;
			}
			//--------------------------------------------
			case LED_FIXED:
			{
				LED_SetColor(ColorFixed);
				break;
			}
			//--------------------------------------------
			default:
			{
				break;
			}
		}
	}
}

//==================================================================================================
//
//==================================================================================================
static void LED_SetColor(uint8_t Led_Color)
{
	switch(Led_Color)
	{
		case LED_COLOR_OFF:
		{
			LED_SetRGB(0,0,0);
			break;
		}
		case LED_COLOR_WHITE:
		{
			LED_SetRGB(255,255,255);
			break;
		}
		case LED_COLOR_LIGHT_BLUE:
		{
			LED_SetRGB(50,50,255);
			break;
		}
		case LED_COLOR_BLUE:
		{
			LED_SetRGB(0,0,255);
			break;
		}
		case LED_COLOR_LIGHT_GREEN:
		{
			LED_SetRGB(50,255,50);
			break;
		}
		case LED_COLOR_GREEN:
		{
			LED_SetRGB(0,255,0);
			break;
		}
		case LED_COLOR_YELLOW:
		{
			LED_SetRGB(255,255,0);
			break;
		}
		case LED_COLOR_ORANGE:
		{
			LED_SetRGB(255,50,0);
			break;
		}
		case LED_COLOR_RED:
		{
			LED_SetRGB(255,0,0);
			break;
		}
		case LED_COLOR_PINK:
		{
			LED_SetRGB(255,0,20);
			break;
		}
		default:
		{
			LED_SetRGB(0,0,0);
			break;
		}
	}

	CurrentColor = Led_Color;
}

//==================================================================================================
//
//==================================================================================================
void LED_SetFixedColor(uint8_t Led_Color)
{
	ColorFixed = Led_Color;
}

//==================================================================================================
//
//==================================================================================================
uint8_t LED_GetColor()
{
	return CurrentColor;
}

//==================================================================================================
//
//==================================================================================================
void LED_SetMode(uint8_t Led_Mode)
{
	LedMode = Led_Mode;
}

//==================================================================================================
//
//==================================================================================================
static void LED_SetRGB(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	if(gRed < Red)	        gRed++;
	else if(gRed > Red)	    gRed--;

	if(gGreen < Green)	    gGreen++;
	else if(gGreen > Green)	gGreen--;

	if(gBlue < Blue)	    gBlue++;
	else if(gBlue > Blue)	gBlue--;

	PWM_SetDutyCycle (PWM_CHANNEL_LED_RED,   (100 * gRed  )/ 255 );
	PWM_SetDutyCycle (PWM_CHANNEL_LED_GREEN, (100 * gGreen)/ 255 );
	PWM_SetDutyCycle (PWM_CHANNEL_LED_BLUE,  (100 * gBlue )/ 255 );
}


//==================================================================================================
//
//==================================================================================================
static uint8_t LED_ReturnColorByPower(uint8_t Power)
{
	uint8_t ret = LED_COLOR_OFF;

	// 0-12% -> WHITE
	if((Power>=0) && (Power<=12))
	{
		ret = LED_COLOR_WHITE;
	}
	// 13-25% -> LIGHT BLUE
	else if((Power>=13) && (Power<=25))
	{
		ret = LED_COLOR_LIGHT_BLUE;
	}
	// 26-38% -> BLUE
	else if((Power>=26) && (Power<=38))
	{
		ret = LED_COLOR_BLUE;
	}
	// 39-51% -> LIGHT GREEN
	else if((Power>=39) && (Power<=51))
	{
		ret = LED_COLOR_LIGHT_GREEN;
	}
	// 52-64% -> GREEN
	else if((Power>=52) && (Power<=64))
	{
		ret = LED_COLOR_GREEN;
	}
	// 65-77% -> YELLOW
	else if((Power>=65) && (Power<=77))
	{
		ret = LED_COLOR_YELLOW;
	}
	// 78-90% -> ORANGE
	else if((Power>=78) && (Power<=90))
	{
		ret = LED_COLOR_ORANGE;
	}
	// 90-100% -> RED
	else if((Power>90))
	{
		ret = LED_COLOR_RED;
	}

	return ret;
}

//==================================================================================================
//
//==================================================================================================
static uint8_t LED_ReturnColorByTemperature(int16_t Temperature)
{
	uint8_t ret = LED_COLOR_OFF;

	// less than 30 degrees -> LIGHT BLUE
	if((Temperature<=300) )
	{
		ret = LED_COLOR_LIGHT_BLUE;
	}
	// 31-32 degrees -> BLUE
	else if((Temperature>=310) && (Temperature<=320))
	{
		ret = LED_COLOR_BLUE;
	}
	// 33-34 degrees -> LIGHT GREEN
	else if((Temperature>=330) && (Temperature<=340))
	{
		ret = LED_COLOR_LIGHT_GREEN;
	}
	// 35-36 degrees -> GREEN
	else if((Temperature>=350) && (Temperature<=360))
	{
		ret = LED_COLOR_GREEN;
	}
	// 37-38 degrees -> YELLOW
	else if((Temperature>=370) && (Temperature<=380))
	{
		ret = LED_COLOR_YELLOW;
	}
	// 39-40% -> ORANGE
	else if((Temperature>=390) && (Temperature<=400))
	{
		ret = LED_COLOR_ORANGE;
	}
	// more than 40 degrees -> RED
	else if((Temperature>400))
	{
		ret = LED_COLOR_RED;
	}

	return ret;
}

//==================================================================================================
//
//==================================================================================================
static void LED_SetColorByPower(uint8_t Power)
{
	LED_SetColor(LED_ReturnColorByPower(Power));
}

//==================================================================================================
//
//==================================================================================================
static void LED_SetColorByTemperature(int16_t Temperature)
{
	LED_SetColor(LED_ReturnColorByTemperature(Temperature));
}
