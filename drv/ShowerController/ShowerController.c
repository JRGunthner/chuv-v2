#include "ShowerController.h"
#include "main.h"
#include "Flowmeter.h"
#include "Controller.h"
#include "NTC.h"
#include "IrDA.h"
#include "Buzzer.h"
#include "Screen.h"
#include "User.h"
#include "LED.h"
#include "eeprom.h"
#include "Touch.h"

//-------------------------------------------
//
static xQueueHandle CommandsQueueHandler;
#define COMMANDS_QUEUE_SIZE 1

//-------------------------------------------
//
static uint8_t TakingShower = SHOWER_TIME_IDLE;

void ShowerController_RemoteControl(void)
{
	uint32_t Data[COMMANDS_QUEUE_SIZE];
	uint8_t  UserIndex = 0;
	uint8_t  PowerSetPoint = 0;
	uint16_t TemperatureSetPoint = 0;

	IrDA_Init(&CommandsQueueHandler);

	Flowmeter_ForceGetFlow(0);

	for(;;)
	{
		xQueueReceive(CommandsQueueHandler, &Data, portMAX_DELAY);

		Buzzer_Beep(25, 1);

		UserIndex = User_GetIndex();
		PowerSetPoint = User_GetPowerSetPoint(UserIndex);
		TemperatureSetPoint = User_GetTemperatureSetPoint(UserIndex);

		switch(Data[0])
		{
			//----------------------------------------------------------------------------------------------
			case KEY_MENU:
			{
				Screen_Show(SCREEN_MENU);
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_UP:
			{
				switch(User_GetShowerMode(UserIndex))
				{
					case SHOWER_MODE_POWER:
					{
						if(PowerSetPoint < 9)
						{
							PowerSetPoint++;
							User_SetPowerSetPoint(PowerSetPoint, UserIndex);
						}
						else
						{
							Buzzer_Beep(40, 3);
						}

						Screen_Show(SCREEN_POWER_SETPOINT);
						break;
					}
					case SHOWER_MODE_TEMPERATURE:
					{
						if(TemperatureSetPoint < MAX_SETPOINT) // MAX_SETPOINT = 455
						{
							TemperatureSetPoint += 10;
							User_SetTemperatureSetPoint(TemperatureSetPoint, UserIndex);
						}
						else
						{
							Buzzer_Beep(40, 3);
						}

						Screen_Show(SCREEN_TEMPERATURE_SETPOINT);
						break;
					}
					/*case SHOWER_MODE_ADJUST:
					{
						if(PasswordDigit1 < 0xee)
						{
							PasswordDigit1++;
						}
						else
						{
							PasswordDigit1 = 0;
						}
						break;
					}*/

					default:
					{
						Buzzer_Beep(500, 3);
						break;
					}
				}

				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_DOWN:
			{
				switch(User_GetShowerMode(UserIndex))
				{
					case SHOWER_MODE_POWER:
					{
						if(PowerSetPoint >0)
						{
							PowerSetPoint--;
							User_SetPowerSetPoint(PowerSetPoint, UserIndex);
						}
						else
						{
							Buzzer_Beep(40, 3);
						}

						Screen_Show(SCREEN_POWER_SETPOINT);
						break;
					}
					case SHOWER_MODE_TEMPERATURE:
					{
						/* Lower temperature must be equal to the inlet temperature */
						if(TemperatureSetPoint > ((NTC_GetWaterTemperature(TEMPERATURE_INLET) > MIN_SETPOINT) ? NTC_GetWaterTemperature(TEMPERATURE_INLET) : MIN_SETPOINT))
						{
							TemperatureSetPoint -= 10;
							User_SetTemperatureSetPoint(TemperatureSetPoint, UserIndex);
						}
						else
						{
							Buzzer_Beep(40, 3);
						}

						Screen_Show(SCREEN_TEMPERATURE_SETPOINT);
						break;
					}
					/*case SHOWER_MODE_ADJUST:
					{
						if(PasswordDigit2 < 0xee)
						{
							PasswordDigit2++;
						}
						else
						{
							PasswordDigit2 = 0;
						}
						break;
					}*/

					default:
					{
						Buzzer_Beep(500, 3);
						break;
					}
				}

				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_USER:
			{
				if(UserIndex >= 5)
				{
					User_SetIndex(0);
				}
				else
				{
					User_SetIndex(++UserIndex);
				}

				Screen_Show(SCREEN_USER);
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_LED_OFF:
			{
				static uint8_t Last_LedMode;

				if(User_GetLedMode(UserIndex) != LED_OFF)
				{
					Last_LedMode = User_GetLedMode(UserIndex);
					User_SetLedMode(LED_OFF, UserIndex);
					Screen_Show(SCREEN_MESSAGE_LED_OFF);
				}
				else
				{
					User_SetLedMode(Last_LedMode, UserIndex);
					Screen_Show(SCREEN_MESSAGE_LED_ON);
				}
				LED_SetMode(User_GetLedMode(UserIndex));
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_LED_AUTO:
			{
				if(User_GetLedMode(UserIndex) == LED_POWER)
				{
					User_SetLedMode(LED_TEMPERATURE, UserIndex);
					Screen_Show(SCREEN_MESSAGE_LED_TEMPERATURE);
				}
				else if(User_GetLedMode(UserIndex) == LED_TEMPERATURE)
				{
					User_SetLedMode(LED_AUTO, UserIndex);
					Screen_Show(SCREEN_MESSAGE_LED_AUTO);
				}
				else if(User_GetLedMode(UserIndex) == LED_AUTO)
				{
					User_SetLedMode(LED_FIXED, UserIndex);
					User_SetLedColor(LED_GetColor(), UserIndex);
					LED_SetFixedColor(User_GetLedColor(UserIndex));

					Screen_Show(SCREEN_MESSAGE_LED_FIXED);
				}
				else if(User_GetLedMode(UserIndex) == LED_FIXED)
				{
					User_SetLedMode(LED_POWER, UserIndex);
					Screen_Show(SCREEN_MESSAGE_LED_POWER);
				}
				else
				{
					Screen_Show(SCREEN_MESSAGE_LED_OFF);
				}

				LED_SetMode(User_GetLedMode(UserIndex));
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_BIP:
			{
				if(User_GetBuzzer(UserIndex) == BUZZER_DISABLE)
				{
					User_SetBuzzer(BUZZER_ENABLED, UserIndex);
					Screen_Show(SCREEN_MESSAGE_BIP_ON);
					Buzzer_Control(BUZZER_ENABLED);
				}
				else
				{
					User_SetBuzzer(BUZZER_DISABLE, UserIndex);
					Screen_Show(SCREEN_MESSAGE_BIP_OFF);
					Buzzer_Control(BUZZER_DISABLE);
				}

				break;
			}

			//----------------------------------------------------------------------------------------------
			case KEY_TEMPERATURE:
			{
				if(User_GetShowerMode(UserIndex) == SHOWER_MODE_POWER)
				{
					Controller_ResetPID();
				}

				User_SetShowerMode(SHOWER_MODE_TEMPERATURE, UserIndex);
				Screen_Show(SCREEN_TEMPERATURE);
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_POWER:
			{
				User_SetShowerMode(SHOWER_MODE_POWER, UserIndex);
				Screen_Show(SCREEN_POWER_SETPOINT);
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_TIMER: // Senha
			{
				/* The user can only enter in SHOWER ADJUST if the shower is in Idle Mode */
				/*if(TakingShower == SHOWER_TIME_IDLE)
				{
					//User[User_GetIndex()].ShowerMode = SHOWER_MODE_ADJUST;
					Screen_Show(SCREEN_PASSWORD);
				}*/

				//EnableTimerAdjust = true;
				break;
			}
			//----------------------------------------------------------------------------------------------
			case KEY_LITERS:
			{
				Screen_Show(SCREEN_LITERS);
				break;
			}
			//----------------------------------------------------------------------------------------------
			case E1:
			{
				EE_FlashErase(); // Clear Emulated EEPROM variables
				break;
			}
			//----------------------------------------------------------------------------------------------
			case E2:
			{
				break;
			}
			//----------------------------------------------------------------------------------------------
			case A1:
			{
				// Desliga a leitura do fluxômetro e força um fluxo
				Flowmeter_ForceGetFlow(350);
				break;
			}
			//----------------------------------------------------------------------------------------------
			case A3:
			{
				// Habilita a leitura do fluxômetro
				Flowmeter_ForceGetFlow(0);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	vTaskDelete(NULL);
}

void ShowerController_Init(void)
{
	CommandsQueueHandler = xQueueCreate(COMMANDS_QUEUE_SIZE, sizeof(uint32_t));
	TOUCH_Init(&CommandsQueueHandler);
}

void ShowerController_Task(void)
{
	ShowerController_RemoteControl();
	TOUCH_Task();
}

void ShowerController_SetShowerState(uint8_t ShowerState)
{
	TakingShower = ShowerState;
}

uint8_t ShowerController_GetShowerState(void)
{
	return TakingShower;
}
