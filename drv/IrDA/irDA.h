#ifndef __IRDA_H
#define __IRDA_H

#include "stm32f0xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*
 +------------()------------+
 |                          |
 |  +----+  +----+  +----+  |
 |  ¦ A1 |  | A2 |  | A3 |  |
 |  +----+  +----+  +----+  |
 |  +----+  +----+  +----+  |
 |  ¦ B1 |  | B2 |  | B3 |  |
 |  +----+  +----+  +----+  |
 |  +----+  +----+  +----+  |
 |  ¦ C1 |  | C2 |  | C3 |  |
 |  +----+  +----+  +----+  |
 |  +----+  +----+  +----+  |
 |  ¦ D1 |  | D2 |  | D3 |  |
 |  +----+  +----+  +----+  |
 |  +----+  +----+  +----+  |
 |  ¦ E1 |  | E2 |  | E3 |  |
 |  +----+  +----+  +----+  |
 |  +----+  +----+  +----+  |
 |  ¦ F1 |  | F2 |  | F3 |  |
 |  +----+  +----+  +----+  |
 |  +----+  +----+  +----+  |
 |  ¦ G1 |  | G2 |  | G3 |  |
 |  +----+  +----+  +----+  |
 |                          |
 +--------------------------+
 */

//-------------------------------
// Remote Control Key codes
#define A1 0x24DBB04F // __Not used
#define A2 0x24DBB24D // __Not used
#define A3 0x24DB32CD // __Not used

#define B1 0x24DBE817 // __Not used
#define B2 0x24DB51AE // KEY_UP				618353070
#define B3 0x24DB12ED // __Not used

#define C1 0x24DB817E // KEY_TEMPERATURE
#define C2 0x24DBA15E // KEY_MENU
#define C3 0x24DB9966 // KEY_POWER

#define D1 0x24DBA05F // __Not used
#define D2 0x24DB09F6 // KEY_DOWN			618334710
#define D3 0x24DB08F7 // __Not used

#define E1 0x24DB10EF // KEY_CALIBRACAO
#define E2 0x24DB38C7 // __Not used
#define E3 0x24DBA857 // KEY_HIDDEN_MENU

#define F1 0x24DBC837 // KEY_TIMER
#define F2 0x24DBC43B // KEY_LITERS
#define F3 0x24DBEC13 // KEY_USER

#define G1 0x24DBC23D // KEY_BIP
#define G2 0x24DB9A65 // KEY_LED_OFF
#define G3 0x24DBC13E // KEY_LED_AUTO

//-------------------------------
// Remote Control Key Functions
#define KEY_UP           B2
#define KEY_DOWN         D2
#define KEY_TEMPERATURE  C1
#define KEY_MENU         C2
#define KEY_POWER        C3
#define KEY_CALIBRACAO   E1
#define KEY_HIDDEN_MENU  E3
#define KEY_TIMER        F1
#define KEY_LITERS       F2
#define KEY_USER         F3
#define KEY_BIP          G1
#define KEY_LED_OFF      G2
#define KEY_LED_AUTO     G3

//-------------------------------
// Functions Definitions
void IrDA_Init(xQueueHandle *QueueHandler);
void IrDA_EnableIRQ(void);
void IrDA_DisableIRQ(void);

#endif /* __IRDA_H */
