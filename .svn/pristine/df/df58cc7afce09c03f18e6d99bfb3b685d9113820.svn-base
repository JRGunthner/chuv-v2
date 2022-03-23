#ifndef __FLOWMETER_H
#define __FLOWMETER_H

#include "stm32f0xx.h"

#define FLOW_ON  180
#define FLOW_OFF 150

//-------------------------------
// Functions Definitions
void Flowmeter_Init(void);
void Flowmeter_EnableIRQ(void);
void Flowmeter_DisableIRQ(void);

void Flowmeter_Task(void);

// -->> JUST TO DEBUG <<-- FUNCTION THAT FORCES THE FLOW VALUE
void Flowmeter_ForceGetFlow(uint16_t ForcedFlow);

uint16_t Flowmeter_GetFlow();
uint32_t Flowmeter_GetFrequency();
int16_t  Flowmeter_GetFlowDerivative();

#endif /* __FLOWMETER_H */
