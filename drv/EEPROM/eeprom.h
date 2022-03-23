/**
******************************************************************************
* @file    STM32F0xx_EEPROM_Emulation/inc/eeprom.h 
* @author  MCD Application Team
* @version V1.0.0
* @date    29-May-2012
* @brief   This file contains all the functions prototypes for the EEPROM 
*          emulation firmware library.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported constants --------------------------------------------------------*/
/* Define the size of the sectors to be used */
#define PAGE_SIZE             ((uint32_t)0x0400)  /* Page size = 1KByte */

/* EEPROM start address in Flash */
#define EEPROM_START_ADDRESS  ((uint32_t)0x0800F800) /* EEPROM emulation start address:
														from sector2, after 62KByte of used
														Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS    ((uint32_t)(EEPROM_START_ADDRESS + 0x0000))
#define PAGE0_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS    ((uint32_t)(EEPROM_START_ADDRESS + 0x0400))
#define PAGE1_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                 ((uint16_t)0x0000)
#define PAGE1                 ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE         ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                ((uint16_t)0xFFFF)     /* Page is empty */
#define RECEIVE_DATA          ((uint16_t)0xEEEE)     /* Page is marked to receive data */
#define VALID_PAGE            ((uint16_t)0x0000)     /* Page containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE  ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE   ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL             ((uint8_t)0x80)

/* Variables' number */
#define NB_OF_VAR ((uint8_t)0x03)

/* Virtual address defined by the user: 0xFFFF value is prohibited */
#define EEPROM_ADDRESS_LITERS		((uint16_t)0x1111) // Litros consumidos
#define EEPROM_ADDRESS_HOUERS		((uint16_t)0x2222) // Somat�rio de horas em funcionamento
#define EEPROM_ADDRESS_NUMBER		((uint16_t)0x3333) // N�mero de banhos

#define EEPROM_ADDRESS_MAX_POT		((uint16_t)0x4444) // Pot�ncia m�xima registrada
#define EEPROM_ADDRESS_MIN_POT		((uint16_t)0x5555) // Pot�ncia m�nima registrada
#define EEPROM_ADDRESS_MID_POT		((uint16_t)0x6666) // Pot�ncia m�dia registrada

#define EEPROM_ADDRESS_MAX_IN_TEMP	((uint16_t)0x7777) // Temperatura m�xima de entrada
#define EEPROM_ADDRESS_MIN_IN_TEMP	((uint16_t)0x8888) // Temperatura m�nima de entrada
#define EEPROM_ADDRESS_MID_IN_TEMP	((uint16_t)0x9999) // Temperatura m�dia de entrada

#define EEPROM_ADDRESS_MAX_OUT_TEMP	((uint16_t)0xAAAA) // Temperatura m�xima de sa�da
#define EEPROM_ADDRESS_MIN_OUT_TEMP	((uint16_t)0xBBBB) // Temperatura m�nima de sa�da
#define EEPROM_ADDRESS_MID_OUT_TEMP	((uint16_t)0xCCCC) // Temperatura m�dia de sa�da

#define EEPROM_ADDRESS_TEST			((uint16_t)0xDDDD) // Endere�o para teste


/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t EE_Init(void);
uint16_t EE_ReadVariable (uint16_t VirtAddress, uint16_t* Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t  Data);
void EE_FlashErase(void);

#endif /* __EEPROM_H */

/*
 * (x) Dados a serem armazenados
 *
 *	01 - Litros consumidos:
 *			Somat�rio do consumo de �gua de todos os banhos, em l [litros]
 *
 *	02 - Somat�rio de horas em funcionamento:
 *			Somat�rio de tempo de todos os banhos, em h [horas]
 *
 *	03 - N�mero de banhos:
 *			Cumulativo de todos os banhos realizados
 *
 *	04 - Pot�ncia m�xima registrada:
 *			Maior pot�ncia de banho registrada, em kW [quiloWats]
 *
 *	05 - Pot�ncia m�nima registrada:
 *			Valor medio de todas pot�ncias de banho registradas, em kW [quiloWats]
 *
 *	06 - Pot�ncia m�dia registrada:
 *			Menor pot�ncia de banho registrada, em kW [quiloWats]
 *
 *	07 - Temperatura m�xima de entrada:
 *			Temperatura m�xima de entrada registrada, em �C [graus Celcius]
 *
 *	08 - Temperatura m�nima de entrada:
 *			Temperatura m�nima de entrada registrada, em �C [graus Celcius]
 *
 *	09 - Temperatura m�dia de entrada:
 *			Temperatura m�dia de entrada registrada, em �C [graus Celcius]
 *
 *	10 - Temperatura m�xima de sa�da:
 *			Temperatura m�xima de sa�da registrada, em �C [graus Celcius]
 *
 *	11 - Temperatura m�nima de sa�da:
 *			Temperatura m�nima de sa�da registrada, em �C [graus Celcius]
 *
 *	12 - Temperatura m�dia de sa�da:
 *			Temperatura m�dia de sa�da registrada, em �C [graus Celcius]
 *
 *
 *	Grava os 12 valores na flash a cada intervalo de 24h entre banhos OU ap�s tr�s
 *	banhos com intervalo de tempo, do primeiro ao terceiro, inferior a 24h.
 *	Se ocorrerem dois banhos dentro das 24h, os valores s�o acumulados no programa.
 *	Findando o tempo, os valores acumulados e calculados s�o gravados na flash.
 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
