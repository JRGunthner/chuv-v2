#include "Screen.h"
#include "NTC.h"
#include "Display.h"
#include "string.h"
#include "TRIAC.h"
#include <stdbool.h>

//-------------------------------
// Screens that are not visible outside screen.c
#define SCREEN_MSG						0xB0
#define SCREEN_MAIN						0xB1
#define SCREEN_GRADE					0xB2 // Nota
#define SCREEN_SHOWER_PERIOD			0xB3 // Tempo de banho
#define SCREEN_WATER_CONSUMPTION		0xB4 // Consumo de água
#define SCREEN_ENERGY_CONSUMPTION		0xB5 // Consumo de energia
#define SCREEN_ESTIMATED_MONTHLY_COST	0xB6 // Consumo estimado mensal

//-------------------------------
// Menu sub screen
#define SCREEN_MENU_RESET	0xA0 // Reset
#define SCREEN_MENU_TE		0xA1 // Temperatura de Entrada
#define SCREEN_MENU_TS		0xA2 // Temperatura de Saída
#define SCREEN_MENU_LT		0xA3 // Litros
#define SCREEN_MENU_VZ		0xA4 // Vazão
#define SCREEN_MENU_TB		0xA5 // Tempo de Banho
#define SCREEN_MENU_LG		0xA6 // Log
#define SCREEN_MENU_CB		0xA7 // Consumo Mensal

//-------------------------------
// variables for SCREEN_MSG
#define SCREEN_BUFFER_LEN 24
uint8_t DisplayBuffer[SCREEN_BUFFER_LEN];
uint8_t iSize = 0;
uint8_t iSizeAux = 0;
static bool EndBuff = false;

static uint8_t TimeoutScreen = 0;
static uint8_t TimeoutScreenBlink = 0;

static uint16_t TimeoutWellcomeMsg = 0;
#define TIMEOUT_WELLCOME_MSG 1200 // 120s - 2min

uint8_t Screen = SCREEN_MAIN;

uint8_t MainSubScreen = SCREEN_MAIN_IDLE;
uint8_t MenuSubScreen = SCREEN_MENU_RESET;
uint8_t NextScreen    = SCREEN_MAIN;

uint8_t blink = 0;

static uint8_t StateLogs = 0;
static uint8_t StatePassword = 0;

//-------------------------------
// Screens Timeouts
#define TIMEOUT_SCREEN_VERSION		40 // 4s
#define TIMEOUT_SCREEN_FLOW			80 // 8s
#define TIMEOUT_SCREEN_SETPOINT		40 // 4s
#define TIMEOUT_SCREEN_USER			40 // 4s
#define TIMEOUT_SCREEN_POWER		40 // 4s
#define TIMEOUT_SCREEN_MENU_MSG		40 // 4s
#define TIMEOUT_SCREEN_MENU_DATA	100// 10s
#define TIMEOUT_SCREEN_LITERS		40 // 4s
#define TIMEOUT_SCREEN_TEMPERATURE	40 // 4s
#define TIMEOUT_SCREEN_PASSWORD		40 // 4s
#define TIMEOUT_SCREEN_DOT_BLINK	4  // 0,4s
#define TIMEOUT_SCREEN_GRADE		3  //
#define TIMEOUT_SCREEN_SHOWER_PERIOD			3 //
#define TIMEOUT_SCREEN_WATER_CONSUMPTION		3 //
#define TIMEOUT_SCREEN_ENERGY_CONSUMPTION		3 //
#define TIMEOUT_SCREEN_ESTIMATED_MONTHLY_COST	3 //

//==================================================================================================
//
//==================================================================================================
/**
  * @brief  Prints in Displays the Firmware Version and the Welcome Message
  * @param  None
  * @retval None
  */
void Screen_Init()
{
	Display_Version(VERSION_MAJOR, VERSION_MINOR);

	vTaskDelay(2000);

	Screen_Show(SCREEN_MESSAGE_WELLCOME);
}

//==================================================================================================
//
//==================================================================================================
void Screen_ShowError()
{
	if(TRIAC_GetError() == TRIAC_ERROR_T1)
	{
		Display_UpdateChar('T', '1'); // Short circuit in TRIAC 1
	}
	if(TRIAC_GetError() == TRIAC_ERROR_T2)
	{
		Display_UpdateChar('T', '2'); // Short circuit in TRIAC 1
	}
	if(TRIAC_GetError() == TRIAC_ERROR_T3)
	{
		Display_UpdateChar('T', '3'); // Short circuit in TRIAC 1 and TRIAC 2
	}
	if(TRIAC_GetError() == TRIAC_ERROR_LOST_EDGES)
	//if(TRIAC_GetError() == TRIAC_LOST_EDGES_ERROR)
	{
		Display_UpdateChar('E', '8');
	}
}

//==================================================================================================
//
//==================================================================================================
void Screen_ShowMessage(char *Buff, uint8_t SizeBuff)
{
	memset(DisplayBuffer, 0x00, sizeof(DisplayBuffer));
	sprintf((char*)DisplayBuffer, "%s", Buff);
	iSize = SizeBuff;
	iSizeAux = 0;
	Screen = SCREEN_MSG;
}

//==================================================================================================
//
//==================================================================================================
void Screen_ShowFrase(char *Buff, uint8_t SizeBuff)
{
	memset(DisplayBuffer, 0x00, sizeof(DisplayBuffer));
	sprintf((char*)DisplayBuffer, "%s", Buff);
	iSize = SizeBuff;
	iSizeAux = 0;
	EndBuff = true;
}

bool EndMessage = false;
//==================================================================================================
//
//==================================================================================================
/**
  * @brief	Signals the end of the message written on the Displays.
  * @param  None
  * @retval TRUE if End of Message or FALSE if Message is being written.
  */
bool Screen_GetEndMessage()
{
	return EndMessage;
}

//==================================================================================================
//
//==================================================================================================
void Screen_Task(uint16_t TSP, uint8_t PSP, uint8_t UI, uint32_t TPSM, uint16_t VWC,
				 uint16_t EMC, uint16_t GRD, uint16_t EC, uint8_t PD1, uint8_t PD2)
{
	// TSP	= Temperature Set Point
	// PSP	= Power Set Point
	// UI	= User Index
	// TPSM = Total Period Shower Minutes
	// VWC	= Volume of Water Consumed
	// EMC	= Estimated Monthly Cost
	// GRD	= Grade
	// EC	= Energy Consumption
	// PD1	= Password Digit 1
	// PD2	= Password Digit 2

	//static uint8_t PreviousPD1 = 0, PreviousPD2 = 0;

	if((TRIAC_GetError() != TRIAC_OK) && (TRIAC_GetError() != TRIAC_ERROR_LOST_EDGES))
	{
		Screen_ShowError();
	}
	else
	{
		switch(Screen)
		{
			//-------------------------------------------- Write message
			case SCREEN_MSG:
			{
				EndMessage = false;
				StateLogs = 0;
				Display_UpdateChar(DisplayBuffer[iSizeAux], DisplayBuffer[iSizeAux + 1]);
				iSizeAux++;
				if(iSizeAux > iSize)
				{
					iSize = 0;
					iSizeAux = 0;
					TimeoutScreen = 0;
					EndMessage = true;
					Screen = NextScreen;
				}
				break;
			}

			//-------------------------------------------- Show main screen
			/* Main operation
			 *
			 * -POWER - It shows in displays: "Px", x={0,1,...,9}
			 *
			 * -TEMPERATURE - It shows in displays the water temperature
			 *
			 * -IDLE - It shows in displays two things:
			 * 		every time it shows "--". Times and times it shows the Welcome Message.
			 */
			case SCREEN_MAIN:
			{
				if(MainSubScreen == SCREEN_MAIN_POWER) // POWER
				{
					Display_UpdateChar('P', PSP + 0x30);
				}
				else if(MainSubScreen == SCREEN_MAIN_TEMPERATURE) // TEMPERATURE
				{
					Display_UpdateValue(NTC_GetWaterTemperature(TEMPERATURE_OUTLET));
				}
				else // IDLE, without operation
				{
					if(TimeoutWellcomeMsg > TIMEOUT_WELLCOME_MSG)
					{
						TimeoutWellcomeMsg = 0;
						Screen_ShowMessage(MSG_WELLCOME, MSG_WELLCOME_SIZE);
						NextScreen = SCREEN_MAIN;
					}
					else
					{
						Display_UpdateChar('-','-');
						TimeoutWellcomeMsg++;
					}
				}
				break;
			}
			//-------------------------------------------- Show water flow
			case SCREEN_FLOW:
			//	Shows the water flow in displays
			{
				Display_UpdateValue(Flowmeter_GetFlow()/10);
				if(TimeoutScreen++> TIMEOUT_SCREEN_FLOW)
				{
					TimeoutScreen = 0;
					Screen = SCREEN_MAIN;
				}
				break;
			}
			//-------------------------------------------- Show temperature
			case SCREEN_TEMPERATURE:
			{
				Display_UpdateValue(NTC_GetWaterTemperature(TEMPERATURE_OUTLET));
				if(TimeoutScreen++> TIMEOUT_SCREEN_TEMPERATURE)
				{
					TimeoutScreen = 0;
					Screen = SCREEN_MAIN;
				}
				break;
			}
			//-------------------------------------------- Show power set point
			case SCREEN_POWER_SETPOINT:
			{
				Display_UpdateChar('P', PSP + 0x30 );
				if(TimeoutScreen++> TIMEOUT_SCREEN_POWER)
				{
					TimeoutScreen = 0;
					Screen = SCREEN_MAIN;
				}
				break;
			}
			//-------------------------------------------- Show temperature set point
			case SCREEN_TEMPERATURE_SETPOINT:
			{
				Display_UpdateValue(TSP); // Temperature mode

				/* Frequency of dot blink */
				if(TimeoutScreenBlink++> TIMEOUT_SCREEN_DOT_BLINK) // 0,4s
				{
					blink = ~blink;
					// blink = 0x00 -> DOT off
					// blink = 0xFF -> DOT on
					TimeoutScreenBlink = 0;
				}

				Display_UpdatePoints(blink, DISPLAY_POINT_MIDDLE | DISPLAY_POINT_RIGHT);

				/* Time that dots stay blinking */
				if(TimeoutScreen++> TIMEOUT_SCREEN_SETPOINT) // 4s
				{
					blink = 0;
					Display_UpdatePoints(0, DISPLAY_POINT_MIDDLE | DISPLAY_POINT_RIGHT);
					TimeoutScreen = 0;
					Screen = SCREEN_MAIN;
				}
				break;
			}
			//-------------------------------------------- Show user
			case SCREEN_USER:
			{
				Display_UpdateChar('u',(UI + 1) + 0x30);
				if(TimeoutScreen++> TIMEOUT_SCREEN_USER)
				{
					TimeoutScreen = 0;
					Screen = SCREEN_MAIN;
				}
				break;
			}
			//-------------------------------------------- Show litters
			case SCREEN_LITERS:
			{
				Display_UpdateValue(VWC/10);
				if(TimeoutScreen++ > TIMEOUT_SCREEN_LITERS)
				{
					TimeoutScreen = 0;
					TimeoutWellcomeMsg = TIMEOUT_WELLCOME_MSG + 1;
					Screen = SCREEN_MAIN;
				}
				break;
			}
			//-------------------------------------------- Show TIMER ***************TIMER*****************
			case SCREEN_PASSWORD:
			{
				static uint8_t PasswordDigit1, PasswordDigit2;

				switch(StatePassword)
				{
					case 0:
					{
						PasswordDigit1 = PasswordDigit2 = 16; // 16 -->> "_"
						Display_UpdateDecimal(PasswordDigit1, PasswordDigit2); // Print "_ _"

						//PreviousPD1 = PD1;
						//PreviousPD2 = PD2;

						TimeoutScreen = 0;
						StatePassword = 1;
						break;
					}

					case 1:
					{
						if(TimeoutScreen++ > TIMEOUT_SCREEN_PASSWORD)
						{
							TimeoutScreen = 0;

							/*if((PasswordDigit2 == 1) && (PasswordDigit1 == 1)) // Password => "11"
							{
								StatePassword = 2;
							}
							else
							{
								StatePassword = 3; // Shows Welcome Message
							}*/

							StatePassword = 3;
						}
						else
						{
							/*if(PD1 != PreviousPD1)
							{
								PreviousPD1 = PD1;
								TimeoutScreen = 0;

								if(PasswordDigit1++ > 14) // The values only in the 0x00-0x0f range
								{
									PasswordDigit1 = 0;
								}
								Display_UpdateDecimal(PasswordDigit2, PasswordDigit1);
							}
							if(PD2 != PreviousPD2)
							{
								PreviousPD2 = PD2;
								TimeoutScreen = 0;

								if(PasswordDigit2++ > 14) // The values only in the 0x00-0x0f range
								{
									PasswordDigit2 = 0;
								}
								Display_UpdateDecimal(PasswordDigit2, PasswordDigit1);
							}*/

							StatePassword = 1;
						}
						break;
					}
					case 2:
					{
						if(TimeoutScreen++ > TIMEOUT_SCREEN_PASSWORD)
						{
							TimeoutScreen = 0;
							StatePassword = 3;
						}
						else
						{
							/*
							if(PD1 != PreviousPD1)
							{
								PreviousPD1 = PD1;
								TimeoutScreen = 0;

								if(PasswordDigit1++ > 14)
								{
									PasswordDigit1 = 0;
								}
								Display_UpdateDecimal(PasswordDigit2, PasswordDigit1);
							}
							if(PD2 != PreviousPD2)
							{
								PreviousPD2 = PD2;
								TimeoutScreen = 0;

								if(PasswordDigit2++ > 14)
								{
									PasswordDigit2 = 0;
								}
								Display_UpdateDecimal(PasswordDigit2, PasswordDigit1);
							}
							*/

							Display_UpdateChar2(SHORTMSG_PERIOD_ON);
							StatePassword = 2;
						}
						break;
					}

					//-------------------------------------------- End state machine
					case 3:
					{
						Screen = SCREEN_MAIN;
						TimeoutWellcomeMsg = TIMEOUT_WELLCOME_MSG + 1;
						StatePassword = 0;
						break;
					}

					default:
					{
						Screen = SCREEN_MAIN;
						TimeoutWellcomeMsg = TIMEOUT_WELLCOME_MSG + 1;
						StatePassword = 0;
						break;
					}
				}

				break;
			}
			//-------------------------------------------- Show menu
			case SCREEN_MENU:
			{
				TimeoutScreen++;

				if(MenuSubScreen == SCREEN_MENU_TE) // Temperatura de entrada
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_INLET_TEMPERATURE);
					}
					else
					{
						Display_UpdateValue(NTC_GetWaterTemperature(TEMPERATURE_INLET));
					}
				}
				else if(MenuSubScreen == SCREEN_MENU_TS) // Temperatura de saída
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_OUTLET_TEMPERATURE);
					}
					else
					{
						Display_UpdateValue(NTC_GetWaterTemperature(TEMPERATURE_OUTLET));
					}
				}
				else if(MenuSubScreen == SCREEN_MENU_LT) // Litros
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_LITERS);
					}
					else
					{
						Display_UpdateValue(VWC/10);
						//Display_UpdateChar2(SHORTMSG_NOT_APPLIED);
					}
				}
				else if(MenuSubScreen == SCREEN_MENU_VZ) // Vazão
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_FLOW);
					}
					else
					{
						Display_UpdateValue(Flowmeter_GetFlow()/10);
					}
				}
				else if(MenuSubScreen == SCREEN_MENU_TB) // Tempo de banho
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_SHOWER_TIME);
					}
					else
					{
						Display_UpdateTimeValue(TPSM);
					}
				}
				else if(MenuSubScreen == SCREEN_MENU_LG) // Logs
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_LOG);
					}
					else
					{
						Screen = SCREEN_LOGS;
						EndBuff = false;
						StateLogs = 0;
					}
				}
				else if( MenuSubScreen == SCREEN_MENU_CB) // Custo de banho
				{
					if(TimeoutScreen < TIMEOUT_SCREEN_MENU_MSG)
					{
						Display_UpdateChar2(SHORTMSG_MONTH_COMSUMPTION);
					}
					else
					{
						Display_UpdateValue(EMC);
					}
				}

				if(TimeoutScreen > (TIMEOUT_SCREEN_MENU_MSG + TIMEOUT_SCREEN_MENU_DATA))
				{
					Screen = ((MenuSubScreen == SCREEN_MENU_LG) ? SCREEN_LOGS : SCREEN_MAIN);
					MenuSubScreen = SCREEN_MENU_RESET;
					TimeoutScreen = 0;
				}
				break;
			}
			//-------------------------------------------- Show Logs
			case SCREEN_LOGS:
			{
				switch(StateLogs)
				{
					//-------------------------------------------- Show grade
					case 0:
					{
						if(!EndBuff)
						{
							Screen_ShowFrase(MSG_GRADE, MSG_GRADE_SIZE);
						}

						Display_UpdateChar(DisplayBuffer[iSizeAux], DisplayBuffer[iSizeAux + 1]);
						iSizeAux++;

						if(iSizeAux > iSize)
						{
							iSize = 0;
							iSizeAux = 0;
							TimeoutScreen = 0;
							EndBuff = false;
							StateLogs = 1;
						}
						else
						{
							StateLogs = 0;
						}

						break;
					}
					case 1: //************************************ Show grade value
					{
						Display_UpdateValue(GRD);
						if(TimeoutScreen++ > TIMEOUT_SCREEN_GRADE)
						{
							TimeoutScreen = 0;
							StateLogs = 2;
						}
						else
						{
							StateLogs = 1;
						}
						break;
					}
					//-------------------------------------------- Show water consumption
					case 2:
					{
						if(!EndBuff)
						{
							Screen_ShowFrase(MSG_WATER_CONSUMPTION, MSG_WATER_CONSUMPTION_SIZE);
						}

						Display_UpdateChar(DisplayBuffer[iSizeAux], DisplayBuffer[iSizeAux + 1]);
						iSizeAux++;
						if(iSizeAux > iSize)
						{
							iSize = 0;
							iSizeAux = 0;
							TimeoutScreen = 0;
							EndBuff = false;
							StateLogs = 3;
						}
						else
						{
							StateLogs = 2;
						}
						break;
					}
					case 3: //************************************ Show water consumption
					{
						Display_UpdateValue(VWC/10);
						if(TimeoutScreen++ > TIMEOUT_SCREEN_GRADE)
						{
							TimeoutScreen = 0;
							StateLogs = 4;
						}
						else
						{
							StateLogs = 3;
						}
						break;
					}
					//-------------------------------------------- Show shower period
					case 4:
					{
						if(!EndBuff)
						{
							Screen_ShowFrase(MSG_SHOWER_PERIOD, MSG_SHOWER_PERIOD_SIZE);
						}

						Display_UpdateChar(DisplayBuffer[iSizeAux], DisplayBuffer[iSizeAux + 1]);
						iSizeAux++;
						if(iSizeAux > iSize)
						{
							iSize = 0;
							iSizeAux = 0;
							TimeoutScreen = 0;
							EndBuff = false;
							StateLogs = 5;
						}
						else
						{
							StateLogs = 4;
						}
						break;
					}
					case 5: //************************************ Show shower period
					{
						Display_UpdateValue(TPSM);
						if(TimeoutScreen++ > TIMEOUT_SCREEN_GRADE)
						{
							TimeoutScreen = 0;
							StateLogs = 6;
						}
						else
						{
							StateLogs = 5;
						}
						break;
					}
					//-------------------------------------------- Show energy consumption
					case 6:
					{
						if(!EndBuff)
						{
							Screen_ShowFrase(MSG_ENERGY_CONSUMPTION, MSG_ENERGY_CONSUMPTION_SIZE);
						}
						Display_UpdateChar(DisplayBuffer[iSizeAux], DisplayBuffer[iSizeAux + 1]);
						iSizeAux++;
						if(iSizeAux > iSize)
						{
							iSize = 0;
							iSizeAux = 0;
							TimeoutScreen = 0;
							EndBuff = false;
							StateLogs = 7;
						}
						else
						{
							StateLogs = 6;
						}
						break;
					}
					case 7: //************************************ Show energy consumption
					{
						Display_UpdateValue(EC);
						if(TimeoutScreen++ > TIMEOUT_SCREEN_GRADE)
						{
							TimeoutScreen = 0;
							StateLogs = 8;
						}
						else
						{
							StateLogs = 7;
						}
						break;
					}
					//-------------------------------------------- Show estimated monthly cost
					case 8:
					{
						if(!EndBuff)
						{
							Screen_ShowFrase(MSG_ESTIMATED_MONTHLY_COST, MSG_ESTIMATED_MONTHLY_COST_SIZE);
						}
						Display_UpdateChar(DisplayBuffer[iSizeAux], DisplayBuffer[iSizeAux + 1]);
						iSizeAux++;
						if(iSizeAux > iSize)
						{
							iSize = 0;
							iSizeAux = 0;
							TimeoutScreen = 0;
							EndBuff = false;
							StateLogs = 9;
						}
						else
						{
							StateLogs = 8;
						}
						break;
					}
					case 9: //************************************ Show energy consumption
					{
						Display_UpdateValue(EMC);
						if(TimeoutScreen++ > TIMEOUT_SCREEN_GRADE)
						{
							TimeoutScreen = 0;
							StateLogs = 10;
						}
						else
						{
							StateLogs = 9;
						}
						break;
					}

					//-------------------------------------------- End state machine
					case 10:
					{
						Screen = SCREEN_MAIN;
						TimeoutWellcomeMsg = TIMEOUT_WELLCOME_MSG + 1;
						EndBuff = false;
						StateLogs = 0;
						break;
					}
					default:
					{
						Screen = SCREEN_MAIN;
						EndBuff = false;
						StateLogs = 0;
						break;
					}
				}
				break;
			}
		}
	}

    if((Screen == SCREEN_MSG) || (Screen == SCREEN_LOGS))
    {
    	vTaskDelay(600);
    }
    else
    {
    	vTaskDelay(100);
    }
}

//==================================================================================================
//
//==================================================================================================
void Screen_SetMainSubScreen(uint8_t Scr)
{
	MainSubScreen = Scr;
}

//==================================================================================================
//
//==================================================================================================
void Screen_Show(uint8_t Scr)
{
	EndMessage = false;

    switch(Scr)
	{
		//--------------------------------------------
		case SCREEN_VERSION:
		{
			Display_Version(VERSION_MAJOR, VERSION_MINOR);
			// VERSION_MAJOR and VERSION_MINOR defined in "Makefile"
			break;
		}
    	//--------------------------------------------
    	case SCREEN_MENU:
    	{
    		if(MenuSubScreen < SCREEN_MENU_CB)
    		{
    			MenuSubScreen++;
    		}
    		else
    		{
    			MenuSubScreen = SCREEN_MENU_TE;
    		}

    		TimeoutScreen = 0;
    		Screen = SCREEN_MENU;
    		break;
    	}
    	//--------------------------------------------
    	case SCREEN_LOGS:
    	{
    		TimeoutScreen = 0;
    		Screen = SCREEN_LOGS;
    		NextScreen = SCREEN_LOGS;
    		break;
    	}
		//--------------------------------------------
		case SCREEN_FLOW:
		{
			TimeoutScreen = 0;
			Screen = SCREEN_FLOW;
			break;
		}
		//--------------------------------------------
		case SCREEN_TEMPERATURE:
		{
			TimeoutScreen = 0;
			Screen = SCREEN_TEMPERATURE;
			break;
		}
    	//--------------------------------------------
    	case SCREEN_TEMPERATURE_SETPOINT:
    	{
    		TimeoutScreen = 0;
    		Screen = SCREEN_TEMPERATURE_SETPOINT;
    		break;
    	}
		//--------------------------------------------
		case SCREEN_POWER_SETPOINT:
		{
			TimeoutScreen = 0;
			Screen = SCREEN_POWER_SETPOINT;
			break;
		}
		//--------------------------------------------
		case SCREEN_USER:
		{
			TimeoutScreen = 0;
			Screen = SCREEN_USER;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_LED_OFF:
		{
			Screen_ShowMessage(MSG_LED_OFF, MSG_LED_OFF_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_LED_ON:
		{
			Screen_ShowMessage(MSG_LED_ON, MSG_LED_ON_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_LED_TEMPERATURE:
		{
			Screen_ShowMessage(MSG_LED_TEMPERATURE, MSG_LED_TEMPERATURE_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_LED_AUTO:
		{
			Screen_ShowMessage(MSG_LED_AUTO, MSG_LED_AUTO_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_LED_FIXED:
		{
			Screen_ShowMessage(MSG_LED_FIXED, MSG_LED_FIXED_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_LED_POWER:
		{
			Screen_ShowMessage(MSG_LED_POWER, MSG_LED_POWER_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_BIP_ON:
		{
			Screen_ShowMessage(MSG_BIP_ON, MSG_BIP_ON_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_BIP_OFF:
		{
			Screen_ShowMessage(MSG_BIP_OFF, MSG_BIP_OFF_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_MESSAGE_WELLCOME:
		{
			Screen_ShowMessage(MSG_WELLCOME, MSG_WELLCOME_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
		//--------------------------------------------
		case SCREEN_LITERS:
		{
			Screen_ShowMessage(MSG_LITERS, MSG_LITERS_SIZE);
			NextScreen = SCREEN_LITERS;
			break;
		}
		//--------------------------------------------
		case SCREEN_PASSWORD:
		{
			Screen_ShowMessage(MSG_PASSWORD, MSG_PASSWORD_SIZE);
			StatePassword = 0;
			NextScreen = SCREEN_PASSWORD;
			break;
		}
		//--------------------------------------------
		case SCREEN_END_SHOWER:
		{
			Screen_ShowMessage(MSG_END_SHOWER, MSG_END_SHOWER_SIZE);
			StateLogs = 0;
			EndBuff = false;
			NextScreen = SCREEN_LOGS;
			break;
		}
		//--------------------------------------------
		default:
		{
			Screen_ShowMessage(MSG_ERROR, MSG_ERROR_SIZE);
			NextScreen = SCREEN_MAIN;
			break;
		}
	}
}
