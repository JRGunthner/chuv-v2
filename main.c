/* Common includes */
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include <stdbool.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* drv includes */
#include "User.h"
#include "Display.h"
#include "LED.h"
#include "Bluetooth.h"
#include "printf_lib.h"
#include "IrDA.h"
#include "NTC.h"
#include "Voltmeter.h"
#include "Flowmeter.h"
#include "Triac.h"
#include "Buzzer.h"
#include "Debug.h"
#include "RTCC.h"
#include "Controller.h"
#include "AD.h"
#include "Touch.h"
#include "eeprom.h"
#include "Screen.h"
#include "ShowerController.h"

//-------------------------------------------
// Shower Password
uint8_t PasswordDigit1 = 0, PasswordDigit2 = 0;
#define WAIT_MESSAGE 0xff

//-------------------------------------------
// Shower Period variables
static bool ClearShowerTime	= 0;
//static uint8_t TakingShower = 0;
static uint32_t TotalPeriodShowerSeconds = 0;
static uint32_t TotalPeriodShowerMinutes = 0;
/*
#define SHOWER_TIME_IDLE	 0x01
#define SHOWER_TIME_STARTED	 0x02
#define SHOWER_TIME_FINISHED 0x03*/

//-------------------------------------------
// Shower Grade variables
uint16_t ShowerGrade = 0;
static uint16_t EstimatedMonthlyCost = 0;

//-------------------------------------------
// Energy variables
uint16_t TotalPowerConsumption = 0;

//-------------------------------------------
// Logs variables
/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
static uint16_t PowerStored = 0;

//-------------------------------------------
// Flowmeter variables
uint16_t VolumeWaterConsumed = 0;

//-------------------------------------------
// Debug variables
uint16_t print1=0, print2=0, print3=0, print4=0, print5=0,
		 print6=0, print7=0, print8=0, print9=0, print10=0;

//-------------------------------------------
// Debug Defines
#define _DEBUG_
#define _FAST_DEBUG_
//#define _DEBUG_STR_

#if defined(_DEBUG_) || defined(_DEBUG_STR_)
	char str_debug[80];
	//uint8_t debug_show = 4;
    //#define DEBUG_MAX 4
#endif

//-------------------------------------------
// Voltmeter Defines
//#define _VOLTMETER_

//-------------------------------------------
// Shower Logs Defines
#define _LOGS_
//#define _EEPROM_TEST_

//-------------------------------------------
// Shower Time Defines
#define _SHOWERTIME_

//-------------------------------------------
// Shower Controller Defines
#define	_SHOWERCONTROL_

//-------------------------------------------
// Tasks Handlers
xTaskHandle xHandle_10msTask 	= NULL;
xTaskHandle xHandle_ScreenTask  = NULL;
xTaskHandle xHandle_BuzzerTask 	= NULL;
xTaskHandle xHandle_PIDTask 	= NULL;
xTaskHandle xHandle_TriacControllerTask = NULL;

#ifdef _VOLTMETER_
xTaskHandle xHandle_VoltmeterTask = NULL;
#endif

#ifdef _DEBUG_
	xTaskHandle xHandle_DebugTask = NULL;
#endif

#ifdef _LOGS_
	xTaskHandle xHandle_ShowerLogsTask = NULL;
#endif

#ifdef _SHOWERTIME_
	xTaskHandle xHandle_ShowerTimeTask = NULL;
#endif

#ifdef _SHOWERCONTROL_
	xTaskHandle xHandle_ShowerControllerTask = NULL;
#endif

//-------------------------------------------
// Tasks Code
static void v10msTask(void *pvParameters);
static void vScreenTask(void *pvParameters);
static void vTriacControllerTask(void *pvParameters);
static void vBuzzerTask(void *pvParameters);
static void vPIDTask(void *pvParameters);

#ifdef _VOLTMETER_
static void vVoltmeterTask(void *pvParameters); // Voltmeter
#endif

#ifdef _DEBUG_
	static void vDebugTask(void *pvParameters); // Debug
#endif

#ifdef _LOGS_
	static void vShowerLogsTask(void *pvParameters); // Shower logs
#endif

#ifdef _SHOWERTIME_
	static void vShowerTimeTask(void *pvParameters); // Shower period
#endif

#ifdef _SHOWERCONTROL_
	static void vShowerControllerTask(void *pvParameters); // Shower controller
#endif

//-------------------------------------------
// Warnings
// 	(defines in main Makefife)
#if(defined VERSION_MAJOR && VERSION_MAJOR == 0) && (defined VERSION_MINOR && VERSION_MINOR == 0)
	#warning "Verify if firmware version in Makefile is updated (VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD)"
#endif

#if(defined MY_SHOWER)
	#warning "MyShower Compilation"
#elif(defined SHOWER_SHOW)
	#warning "ShowerShow Compilation"
#endif

#if(defined _127V)
	#warning "Shower voltage is 127V"
#elif(defined _220V)
	#warning "Shower voltage is 220V"
#endif

//====================================================================================================
// Calcula a nota do banho do usuário
//====================================================================================================
void Calcula_Nota_Banho(void);

//====================================================================================================
// Main Function
//====================================================================================================
int main(void)
{

#ifdef _DEBUG_STR_
	sprintf_lib(str_debug, "Shower Started - version %d.%d.%d\r\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD); // These defines are in makefile
	Debug_Write(str_debug, strlen(str_debug));
#endif

	SystemInit();  			// CMSIS SystemInit clocks and PLLs
	User_ResetShowerVariables(); // ShowerConfiguration

	RTCC_Init();
	Bluetooth_Init();
	NTC_Init();
	Voltmeter_Init();
	TRIAC_Init();
	ShowerController_Init();

	FLASH_Unlock(); // Unlock the Flash Program Erase controller
	EE_Init();		// Initializes Emulated EEPROM

	Flowmeter_ForceGetFlow(0); // SOMENTE PARA DEBUG

	//-----------------------------------------------------------------------------
	// Buzzer Task
	//-----------------------------------------------------------------------------
    xTaskCreate(vBuzzerTask,
    			"BuzzerTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_BuzzerTask);

	//-----------------------------------------------------------------------------
	// User Interface Task (Display - LED - Flowmeter)
	//-----------------------------------------------------------------------------
    xTaskCreate(v10msTask,
    			"10msTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY + 1,
				xHandle_10msTask);

	//-----------------------------------------------------------------------------
	// Screen Task
	//-----------------------------------------------------------------------------
    xTaskCreate(vScreenTask,
    			"ScreenTask",
				configMINIMAL_STACK_SIZE * 2,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_ScreenTask);

	//-----------------------------------------------------------------------------
	// Debug Task
	//-----------------------------------------------------------------------------
#ifdef _DEBUG_
	xTaskCreate(vDebugTask,
    			"DebugTask",
    			configMINIMAL_STACK_SIZE * 3,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_DebugTask);
#endif // _DEBUG_

	//-----------------------------------------------------------------------------
	// PID Task
	//-----------------------------------------------------------------------------
	xTaskCreate(vPIDTask,
    			"PIDTask",
    			configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_PIDTask);

	//-----------------------------------------------------------------------------
	// Triac Controller Task
	//-----------------------------------------------------------------------------
	xTaskCreate(vTriacControllerTask,
    			"TriacControllerTask",
				configMINIMAL_STACK_SIZE * 2,
				(void *) NULL,
				tskIDLE_PRIORITY + 2,
				xHandle_TriacControllerTask);

	//-----------------------------------------------------------------------------
	// Voltmeter Task
	//-----------------------------------------------------------------------------
#ifdef _VOLTMETER_
	xTaskCreate(vVoltmeterTask,
    			"VoltmeterTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY + 1,
				xHandle_VoltmeterTask);
#endif // _VOLTMETER_

	//-----------------------------------------------------------------------------
	// Shower Logs Task
	//-----------------------------------------------------------------------------
#ifdef _LOGS_
	xTaskCreate(vShowerLogsTask,
    			"ShowerLogsTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_ShowerLogsTask);
#endif // _LOGS_

	//-----------------------------------------------------------------------------
	// Shower Time Task
	//-----------------------------------------------------------------------------
#ifdef _SHOWERTIME_
	xTaskCreate(vShowerTimeTask,
    			"ShowerTimeTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_ShowerTimeTask);
#endif // _SHOWERTIME_

	//-----------------------------------------------------------------------------
	// Shower Controller Task
	//-----------------------------------------------------------------------------
#ifdef _SHOWERCONTROL_
	xTaskCreate(vShowerControllerTask,
    			"ShowerControllerTask",
				configMINIMAL_STACK_SIZE * 2,
				(void *) NULL,
				tskIDLE_PRIORITY + 1,
				xHandle_ShowerControllerTask);
#endif // _SHOWERCONTROL_

	//-----------------------------------------------------------------------------
	// Scheduler
	//-----------------------------------------------------------------------------
    vTaskStartScheduler();

    return 0;
}

//====================================================================================================
// Calcula a nota do banho do usuário utilizando a fórmula,
// 80 - { [ (Litros-35) + (Energia-60) + 3*(tempo-10) ] / 4 }
//====================================================================================================
void Calcula_Nota_Banho(void)
{
    static int16_t	iAcc;
    static uint16_t ucEnergia;
    static uint16_t uiConsumoKWH;
    static uint16_t uiCustoKWH = 700;

    //..................................................................................
    //ucEnergia = ulAccEnergia/ulAccTempoBanho; // Media do percentual de energia consumida
    ucEnergia = TotalPowerConsumption/TotalPeriodShowerMinutes;

    //uiConsumoKWH = uiTempoBanho * ucEnergia * 0.012; // Cálculo em 10*Kwh do consumo do banho atual
    uiConsumoKWH = (TotalPeriodShowerSeconds) * ucEnergia * 0.012;

    EstimatedMonthlyCost = uiConsumoKWH * uiCustoKWH; // Aqui tenho o custo do banho em centavos*10

    EstimatedMonthlyCost = EstimatedMonthlyCost * 0.0309;  // Multilplicando por '(30dias/1000)*1.03' tenho o custo estimado mensal em R$ para o ultimo banho + 3% de água
    //..................................................................................

    //iAcc = uiLitrosBanho -350;
    iAcc = VolumeWaterConsumed -350;

    iAcc = iAcc/10;

    iAcc = iAcc + ((int16_t)ucEnergia - 60 );

    //iAcc = iAcc + ( 3*(signed int)(uiTempoBanho - 10) );
    iAcc = iAcc + (3 * (int16_t)((TotalPeriodShowerSeconds) - 10));

    iAcc = iAcc/2;

    iAcc  = 80 - iAcc;

    if(iAcc > 100)		// nota truncada em 10 para cima
    {
        iAcc = 100;
    }
    else if(iAcc < 0)	// nota truncada em zero para baixo
    {
        iAcc = 0;
    }

    ShowerGrade = iAcc;
}

//====================================================================================================
//
//====================================================================================================
#ifdef _SHOWERCONTROL_
static void vShowerControllerTask(void *pvParameters)
{
	for(;;)
	{
		ShowerController_Task();
	}
	vTaskDelete(NULL);
}
#endif // _SHOWERCONTROL_

//====================================================================================================
//
//====================================================================================================
#ifdef _SHOWERTIME_
static void vShowerTimeTask(void *pvParameters)
{
	RTCC_TimeDateTypeDef ShowerRealTimeDate;

	struct
	{
		uint8_t Date;
		uint8_t Hours;
		uint8_t Minutes;
		uint8_t Seconds;
	}ShowerTime;

	ShowerTime.Date	   = 0;
	ShowerTime.Hours   = 0;
	ShowerTime.Minutes = 0;
	ShowerTime.Seconds = 0;

	ShowerRealTimeDate.WeekDay	= RTC_Weekday_Monday;
	ShowerRealTimeDate.Date		= 29;
	ShowerRealTimeDate.Month	= RTC_Month_October;
	ShowerRealTimeDate.Year		= 15;
	ShowerRealTimeDate.Hours	= 00;
	ShowerRealTimeDate.Minutes	= 00;
	ShowerRealTimeDate.Seconds	= 00;

	RTCC_SetTimeDate(ShowerRealTimeDate);

	for (;;)
	{
		RTCC_GetTimeDate(&ShowerRealTimeDate);

		if(ClearShowerTime) /* Initializes shower time count */
		{
			ShowerTime.Date	   = ShowerRealTimeDate.Date;
			ShowerTime.Hours   = ShowerRealTimeDate.Hours;
			ShowerTime.Minutes = ShowerRealTimeDate.Minutes;
			ShowerTime.Seconds = ShowerRealTimeDate.Seconds;
		}

		if(ShowerController_GetShowerState() == SHOWER_TIME_FINISHED)
		{
			TotalPeriodShowerSeconds =	(uint32_t)(ShowerRealTimeDate.Date * 86400) +
										(uint32_t)(ShowerRealTimeDate.Hours * 3600) +
										(uint32_t)(ShowerRealTimeDate.Minutes * 60) +
										(uint32_t)(ShowerRealTimeDate.Seconds) -
										(uint32_t)(ShowerTime.Date * 86400) -
										(uint32_t)(ShowerTime.Hours * 3600) -
										(uint32_t)(ShowerTime.Minutes * 60)	-
										(uint32_t)(ShowerTime.Seconds);

			TotalPeriodShowerMinutes = TotalPeriodShowerSeconds/60;
		}

				print1 = ShowerRealTimeDate.Minutes;
				print2 = ShowerRealTimeDate.Seconds;

		print3 = TotalPeriodShowerSeconds;
		//print3 = TotalPeriodShowerMinutes;

				print4 = ShowerTime.Minutes;
				print5 = ShowerTime.Seconds;

		vTaskDelay(100);
	}

	vTaskDelete(NULL);
}
#endif // _SHOWERTIME_

//====================================================================================================
//
//====================================================================================================
#ifdef _LOGS_
static void vShowerLogsTask(void *pvParameters)
{
	uint16_t VarDataTab[12] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	uint16_t InletTemperature= 0, OutletTemperature= 0;

	for(;;)
	{
		InletTemperature  = NTC_GetWaterTemperature(TEMPERATURE_INLET);
		OutletTemperature = NTC_GetWaterTemperature(TEMPERATURE_OUTLET);

		if(ShowerController_GetShowerState() == SHOWER_TIME_FINISHED)
		{
			/* Reading first variables values from Flash Virtual Address and storing in VarDataTab[] vector */
			EE_ReadVariable(EEPROM_ADDRESS_LITERS, &VarDataTab[0]);
			EE_ReadVariable(EEPROM_ADDRESS_HOUERS, &VarDataTab[1]);
			EE_ReadVariable(EEPROM_ADDRESS_NUMBER, &VarDataTab[2]);

			EE_ReadVariable(EEPROM_ADDRESS_MAX_POT, &VarDataTab[3]);
			EE_ReadVariable(EEPROM_ADDRESS_MIN_POT, &VarDataTab[4]);
			EE_ReadVariable(EEPROM_ADDRESS_MID_POT, &VarDataTab[5]);

			EE_ReadVariable(EEPROM_ADDRESS_MAX_IN_TEMP, &VarDataTab[6]);
			EE_ReadVariable(EEPROM_ADDRESS_MIN_IN_TEMP, &VarDataTab[7]);
			EE_ReadVariable(EEPROM_ADDRESS_MID_IN_TEMP, &VarDataTab[8]);

			EE_ReadVariable(EEPROM_ADDRESS_MAX_OUT_TEMP, &VarDataTab[9]);
			EE_ReadVariable(EEPROM_ADDRESS_MIN_OUT_TEMP, &VarDataTab[10]);
			EE_ReadVariable(EEPROM_ADDRESS_MID_OUT_TEMP, &VarDataTab[11]);

			/* Write variables values on Flash Virtual Address */
			EE_WriteVariable(EEPROM_ADDRESS_LITERS, VarDataTab[0] + VolumeWaterConsumed);
			EE_WriteVariable(EEPROM_ADDRESS_HOUERS, VarDataTab[1] + TotalPeriodShowerMinutes);
			EE_WriteVariable(EEPROM_ADDRESS_NUMBER, VarDataTab[2]++);

			EE_WriteVariable(EEPROM_ADDRESS_MAX_POT, (VarDataTab[3] > PowerStored) ? VarDataTab[3] : PowerStored);
			EE_WriteVariable(EEPROM_ADDRESS_MIN_POT, (VarDataTab[4] < PowerStored) ? VarDataTab[3] : PowerStored);
			EE_WriteVariable(EEPROM_ADDRESS_MID_POT, (VarDataTab[4] + VarDataTab[3])/2);

			EE_WriteVariable(EEPROM_ADDRESS_MAX_IN_TEMP, (VarDataTab[6] > InletTemperature) ? VarDataTab[6] : InletTemperature);
			EE_WriteVariable(EEPROM_ADDRESS_MIN_IN_TEMP, (VarDataTab[7] < InletTemperature) ? VarDataTab[7] : InletTemperature);
			EE_WriteVariable(EEPROM_ADDRESS_MID_IN_TEMP, (VarDataTab[7] + VarDataTab[6])/2);

			EE_WriteVariable(EEPROM_ADDRESS_MAX_OUT_TEMP, (VarDataTab[9] > OutletTemperature) ? VarDataTab[9] : OutletTemperature);
			EE_WriteVariable(EEPROM_ADDRESS_MIN_OUT_TEMP, (VarDataTab[10] < OutletTemperature) ? VarDataTab[10] : OutletTemperature);
			EE_WriteVariable(EEPROM_ADDRESS_MID_OUT_TEMP, (VarDataTab[10] + VarDataTab[9])/2);
		}

		vTaskDelay(100);
	}

	vTaskDelete(NULL);
}
#endif // _LOGS_

//====================================================================================================
//
//====================================================================================================
#ifdef _VOLTMETER_
static void vVoltmeterTask(void *pvParameters)
{
	uint16_t ADValue = 0;
	uint8_t  VoltmeterState = 0;
	uint16_t VoltValueInitial[3];
	uint8_t  i;
	uint8_t  InitVector = 0;
	uint16_t VoltVectorIndex = 0;
	uint32_t VoltValue = 0;


	//uint16_t *VoltVectorAD;
	//uint16_t VoltVectorIndex = 0;
	//uint32_t VoltValueSum = 0;

	//uint16_t i;
	//uint8_t initVector = 0;

	//for(i = 0; i <= 3; i++) VoltValueInitial[i] = 0; // Clear vector values

	for (;;)
	{
		/*
		if(ADValue > 50)
		{
			//initVector = 1;
				if (initVector);
			VoltVectorAD[VoltVectorIndex++] = ADValue;
			//VoltVectorIndex++;
		}*/

		ADValue = Voltmeter_GetVoltage();	//AD_GetValue(AD_VOLTMETER)
		//if(ADValue <= 50) ADValue = 0;		//Valores cortados do semi ciclo negativo estão por volta de 30b no ADC
		//print2 = AD_GetValue(AD_VOLTMETER);

		//print1 = ADValue;

		switch(VoltmeterState)
		{
			case 0://===============================================================
				//Armazena as primeiras amostras

				//print1 = VoltValueInitial[3];
				for(i = 2; i > 0; i--)
				{
					VoltValueInitial[i] = VoltValueInitial[i-1];
				}

				VoltValueInitial[0] = ADValue;

				//print1 = ADValue;

				print1 = VoltValueInitial[0];
				print2 = VoltValueInitial[1];
				print3 = VoltValueInitial[2];
				print4 = VoltValueInitial[2];
				print5 = VoltValue;

				VoltValue = 0;

				if (!VoltValueInitial[3] ||
					!VoltValueInitial[2] ||
					!VoltValueInitial[1] ||
					!VoltValueInitial[0])
				{
					VoltmeterState = 0;
				}
				else VoltmeterState = 0;

				VoltVectorIndex = 0;
			break;//================================================================

			case 1://===============================================================
				//Verifica se os primeiros valores não são nulos.
				//Desta forma sabe-se que iniciou a senoide.

				//print1 = 900;
				for(i = 0; i <= 3; i++)
				{
					VoltValue += VoltValueInitial[i];
				}

				VoltValue /= 4;

/*
				print1 = VoltValueInitial[0];
				print2 = VoltValueInitial[1];
				print3 = VoltValueInitial[2];
				print4 = VoltValueInitial[3];
				print5 = VoltValue;
*/

				if(VoltValue > 50)
				{
					InitVector = 1; //Inicia a contagem do índice do vetor de amostras
					VoltVectorIndex = 4;
					//VoltValue = 0;	//Clear VoltValue
					VoltmeterState = 2;
				}
				else VoltmeterState = 0;

				VoltmeterState = 3;
			break;//================================================================

			case 2://===============================================================
				if(ADValue >= VoltValue) VoltVectorIndex++;
				if((ADValue < 50) && (VoltVectorIndex > 300)) VoltmeterState = 3;
				else VoltmeterState = 2;
			break;//================================================================

			case 3://===============================================================
				/*print1 = VoltValueInitial[0];
				print2 = VoltValueInitial[1];
				print3 = VoltValueInitial[2];
				print4 = VoltValueInitial[3];
				print5 = VoltValue;*/

				VoltmeterState = 3;
			break;//================================================================

			default://==============================================================
				VoltmeterState = 0;
			break;//================================================================
		}

		vTaskDelay(10);
	}

	vTaskDelete(NULL);
}
#endif

//====================================================================================================
//	Main Shower control
//====================================================================================================
static void vTriacControllerTask(void *pvParameters)
{
	enum {CONTROLLER_ON, CONTROLLER_OFF, CONTROLLER_WAIT} State;
	State = CONTROLLER_OFF;

	uint8_t Power = 0;
	//static uint8_t StateTimerAdjust  = 0;

	uint8_t UserIndex = 0;

	vTaskDelay(2000); // Wait two seconds to begin to work

	LED_Control(LED_DISABLE);

	for(;;)
	{
		TRIAC_LostEdges();

		//---------------------------------------------
		// State Machine Control (Controlled by Water Flow)
		switch(State)
		{
			//----------------------
			case CONTROLLER_ON:
			{
				ClearShowerTime = false;			// Keep count from the "00:00:00"

				if(Flowmeter_GetFlow() < FLOW_OFF)	// If there isn't flow...
				{
					LED_Control(LED_DISABLE);		// Turn LEDs off
					State = CONTROLLER_WAIT;		// Go to CONTROLLER_WAIT state

					ShowerController_SetShowerState(SHOWER_TIME_FINISHED); // Shower finished
					Screen_Show(SCREEN_END_SHOWER);		 // Show in displays the End of Shower message

					Calcula_Nota_Banho();
				}
				break;
			}
			//----------------------
			case CONTROLLER_OFF:
			{
				ShowerController_SetShowerState(SHOWER_TIME_IDLE); // There isn't shower

				if(Flowmeter_GetFlow() > FLOW_ON)	// If there is flow...
				{
					ShowerController_SetShowerState(SHOWER_TIME_STARTED); // Shower time started
					ClearShowerTime	  = true;		// Clear shower time count
					VolumeWaterConsumed	  = 0;		// Clear volume of water consumed count
					TotalPowerConsumption = 0;		// Clear total power consumption count

					Buzzer_Beep(50, 2);				// Give two BIPs
					Controller_ResetPID(); 			// Reset PID
					LED_Control(LED_ENABLED); 		// Turn LEDs on
					Screen_Show(SCREEN_FLOW);		// Show flow in displays

					State = CONTROLLER_ON; 			// Go to CONTROLLER_ON state
				}
				break;
			}
			//----------------------
			case CONTROLLER_WAIT:
			{
				// TODO: implemetar logica para aguardar um tempo ate religar o controle. Isso é devido a energia já acumulada na resistencia
				State = CONTROLLER_OFF;
				break;
			}
			//----------------------
			default:
			{
				State = CONTROLLER_OFF;
				break;
			}
		}

//#define _SHOWER_ADJUST_
/*#ifdef _SHOWER_ADJUST_
		// The user can only enter in SHOWER ADJUST if the shower is in Idle Mode
		if((TakingShower == SHOWER_TIME_IDLE) && (EnableTimerAdjust == true))
		{
			switch(StateTimerAdjust)
			{
				case 0:
				{
					Screen_Show(SCREEN_PASSWORD); // Print " SEnhA "
					StateTimerAdjust = 1;
					break;
				}
				case 1:
				{
					if(Screen_GetEndMessage())
					{
						StateTimerAdjust = 2; // End of message
					}
					else
					{
						StateTimerAdjust = 1;
					}

					break;
				}

				//----------- End state machine
				case 2:
				{
					EnableTimerAdjust = false;
					StateTimerAdjust = 0;
					break;
				}
				default:
				{
					EnableTimerAdjust = false;
					StateTimerAdjust = 0;
					break;
				}
			}
		}
#endif //_SHOWER_ADJUST_*/

		//---------------------------------------------
		// Shows flow variation
		if(Flowmeter_GetFlowDerivative() > 0)
		{
			Screen_Show(SCREEN_FLOW);
		}

		UserIndex = User_GetIndex();

		//---------------------------------------------
		// Apply power according to user configuration SHOWER_MODE_TEMPERATURE or SHOWER_MODE_POWER
		if(State == CONTROLLER_ON)
		{
			if(User_GetShowerMode(UserIndex) == SHOWER_MODE_TEMPERATURE)
			{
				Screen_SetMainSubScreen(SCREEN_MAIN_TEMPERATURE);
				Power = Controller_Task(User_GetTemperatureSetPoint(UserIndex));
			}
			else // SHOWER_MODE_POWER
			{
				Screen_SetMainSubScreen(SCREEN_MAIN_POWER);
				Power = User_GetPowerSetPoint(UserIndex) * 11;
			}

			/* Somatório do volme de água consumido */
			VolumeWaterConsumed += Flowmeter_GetFlow()/60; // TODO: verificar contagem do volume
			print6 = VolumeWaterConsumed/100;

			/* Somatório da energia consumida */
			TotalPowerConsumption += Power;
			print7 = TotalPowerConsumption;
		}
		else
		{
			Screen_SetMainSubScreen(SCREEN_MAIN_IDLE);
			Power = 0;
		}

		//---------------------------------------------
		// TODO: Protecao para sobre tensao

		//---------------------------------------------
		// Max Temperature Protection
		if((NTC_GetWaterTemperature(TEMPERATURE_OUTLET) > MAX_TEMPERATURE) || (NTC_GetWaterTemperature(TEMPERATURE_RESISTANCE) > MAX_TEMPERATURE))
		{
			Power = 0;
		}

		//---------------------------------------------
		// Set Triac power
		TRIAC_SetPower(Power);
		PowerStored = Power;

		//---------------------------------------------
		// Time delay for hysteresis
		if(State ==  CONTROLLER_WAIT)
		{
			vTaskDelay(1000); // TODO: este delay vai depender da temperatura na resistencia.
		}
		else
		{
			vTaskDelay(100);
		}
	}

	vTaskDelete(NULL);
}

//====================================================================================================
//
//====================================================================================================
static void vPIDTask(void *pvParameters)
{
	for (;;)
	{
		PID_Task();

		vTaskDelay(150);
	}
	vTaskDelete(NULL);
}

//====================================================================================================
//
//====================================================================================================
static void vBuzzerTask(void *pvParameters)
{
	Buzzer_Init();
	vTaskDelay(50);
	Buzzer_Beep(50, 2);

	for (;;)
	{
		Buzzer_Task();
	}

	vTaskDelete(NULL);
}

//====================================================================================================
//
//====================================================================================================
static void vScreenTask(void *pvParameters)
{
	uint8_t UserIndex = 0;
	Screen_Init();

	for(;;)
	{
		UserIndex = User_GetIndex();

		Screen_Task(User_GetTemperatureSetPoint(UserIndex),
					User_GetPowerSetPoint(UserIndex),
					UserIndex,
					TotalPeriodShowerMinutes,
					VolumeWaterConsumed,
					EstimatedMonthlyCost,
					EstimatedMonthlyCost,
					TotalPowerConsumption,
					PasswordDigit1,
					PasswordDigit2);
	}

	vTaskDelete(NULL);
}

//====================================================================================================
// User Interface Task (Display - LED - Flowmeter)
//====================================================================================================
static void v10msTask (void *pvParameters)
{
	uint8_t count = 0;
	uint8_t UserIndex = 0;

	UserIndex = User_GetIndex();

	LED_Init();
	LED_SetMode(User_GetLedMode(UserIndex));
	LED_SetFixedColor(User_GetLedColor(UserIndex));


	Display_Init();
	Flowmeter_Init();

	for (;;)
	{
		Display_Task();
		LED_Task();

		if(count > 10) // One reading every 100ms
		{
			count = 0;
				Flowmeter_Task();
			}

		count++;

		vTaskDelay(10);
	}

	vTaskDelete(NULL);
}

//====================================================================================================
// Debug through UART2, RXD/TXD
//====================================================================================================
#ifdef _DEBUG_
static void vDebugTask(void *pvParameters)
{
	#ifndef _FAST_DEBUG_
		static uint16_t i = 0;
	#endif

	Debug_Init();

	for(;;)
	{
		#ifdef _FAST_DEBUG_
			sprintf_lib(str_debug, "%02d:%02d Tmp: %d min %02d:%02d; Vol: %d; Engy %d kW - %d %d %d\n",
						print1,print2,print3,print4,print5,print6,print7,print8,print9,print10);
			Debug_Write(str_debug, strlen(str_debug));

		#else
			sprintf_lib(str_debug, "%d %d\n", AD_GetValue(AD_VOLTAGE_REFERENCE), VoltValue[i++]));//AD_VOLTAGE_REFERENCE
			sprintf_lib(str_debug, "%d\n",  VoltVectorAD[i++]);//AD_VOLTAGE_REFERENCE
			Debug_Write(str_debug, strlen(str_debug));
			if(i==VoltVectorIndex)i=0;

			sprintf_lib(str_debug, "0 %d ", NTC_GetWaterTemperature(TEMPERATURE_INLET));
			Debug_Write(str_debug, strlen(str_debug));

			sprintf_lib(str_debug, "%d ", NTC_GetWaterTemperature(TEMPERATURE_RESISTANCE));
			Debug_Write(str_debug, strlen(str_debug));

			sprintf_lib(str_debug, "%d ", NTC_GetWaterTemperature(TEMPERATURE_OUTLET));
			Debug_Write(str_debug, strlen(str_debug));

			sprintf_lib(str_debug, "%d ", Flowmeter_GetFlow()/10);
			Debug_Write(str_debug, strlen(str_debug));

			sprintf_lib(str_debug, "%d ", Flowmeter_GetFlowDerivative());
			Debug_Write(str_debug, strlen(str_debug));

			sprintf_lib(str_debug, "%d ", TRIAC_GetPower());
			Debug_Write(str_debug, strlen(str_debug));

			sprintf_lib(str_debug, "%d 0\n", User[User_GetIndex()].TemperatureSetPoint);
			Debug_Write(str_debug, strlen(str_debug));
		#endif // _FAST_DEBUG

		vTaskDelay(100);
	}

	vTaskDelete(NULL);
}
#endif

//====================================================================================================
//
//====================================================================================================
void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}

//====================================================================================================
//
//====================================================================================================
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook
	function is called if a stack overflow is detected. */

	taskDISABLE_INTERRUPTS();

	// TODO - Desabilitar as interrupccoes de hardware ou pelo menos a do triac

	for(;;);
}

//====================================================================================================
//
//====================================================================================================
void vApplicationTickHook(void)
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}
