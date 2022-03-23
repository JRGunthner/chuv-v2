#ifndef __SCREEN_H
#define __SCREEN_H

#include "stm32f0xx.h"
#include "Flowmeter.h"
//#include "main.h"
#include "stdio.h"
#include <stdbool.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

//----------------------------
// Headers that translate shower messages
#include "TranslateBR.h"
//#include "TranslateING.h"
//#include "TranslateESP.h"

//-------------------------------
// Screen_Show() defines
#define SCREEN_VERSION					0x01
#define SCREEN_MENU						0x02
#define SCREEN_FLOW						0x07
#define SCREEN_TEMPERATURE				0x08
#define SCREEN_TEMPERATURE_SETPOINT		0x09
#define SCREEN_POWER_SETPOINT			0x10
#define SCREEN_MESSAGE_LED_OFF			0x11
#define SCREEN_MESSAGE_LED_ON			0x12
#define SCREEN_MESSAGE_LED_TEMPERATURE	0x13
#define SCREEN_MESSAGE_LED_AUTO			0x14
#define SCREEN_MESSAGE_LED_FIXED		0x15
#define SCREEN_MESSAGE_LED_POWER		0x16
#define SCREEN_MESSAGE_BIP_ON			0x18
#define SCREEN_MESSAGE_BIP_OFF			0x19
#define SCREEN_MESSAGE_WELLCOME			0x20
#define SCREEN_PASSWORD					0x23
#define SCREEN_USER						0x24
#define SCREEN_LITERS					0x25
#define SCREEN_SHOWER_END				0x26
#define SCREEN_SHOWER_REPORT			0x27
#define SCREEN_LOGS						0x28
#define SCREEN_END_SHOWER				0x29

//-------------------------------
// Screen_SetMainScreen() defines
#define SCREEN_MAIN_IDLE				0x30
#define SCREEN_MAIN_TEMPERATURE			0x31
#define SCREEN_MAIN_POWER				0x32
#define SCREEN_MAIN_ERROR				0x33

//-------------------------------
// Functions Definitions

/**
  * @brief  Prints in Displays the Firmware Version and the first Welcome Message
  * @param  None
  * @retval None
  */
void Screen_Init();

/**
  * @brief
  * @param  TSP: Temperature Set Point.
  * 				This parameter must be set to a value in the 200-450 range. // Verificar se a faixa está correta
  * @param  PSP: Power Set Point.
  * 				This parameter must be set to a value in the 0-9 range.
  * @param  UI:  User Index.
  * 				This parameter must be set to a value in the 1-6 range.
  * @param  TPSM:  Total Period Shower Minutes.
  * 				This parameter must be set to a value in the 0-99 range.
  * @param  VWC: Volume Water Consumed.
  * 				This parameter must be set to a value in the 0-999 range.
  * @param  EMC: Estimated Monthly Cost.
  * 				This parameter must be set to a value in the 0-999 range.
  * @param  GRD: Grade.
  * 				This parameter must be set to a value in the xx-yy range.
  * @param  EC: ENERGY_CONSUMPTION.
  * 				This parameter must be set to a value in the xx-yy range.
  * @param  PD1: Password Digit 1.
  * 				This parameter must be set to a value in the 0x00-0x0f range.
  * @param  PD2: Password Digit 2.
  * 				This parameter must be set to a value in the 0x00-0x0f range.
  * @retval None
  */
void Screen_Task(uint16_t TSP, uint8_t PSP, uint8_t UI, uint32_t TPSM, uint16_t VWC,
		 	 	 uint16_t EMC, uint16_t GRD, uint16_t EC, uint8_t PD1, uint8_t PD2);

void Screen_Show(uint8_t Scr);
bool Screen_GetEndMessage();
void Screen_SetMainSubScreen(uint8_t Scr);

#endif /* __SCREEN_H */
