#include "Voltmeter.h"
#include "AD.h"

//bit blTensaoOk;

//xType xSemaforoAD;

//static uint16_t AD_Volt_Vector[100];
//static uint16_t acont;

//uint16_t uiVetorTensao;
//uint16_t uiTensao;

//extern uint16_t uiTensao;

#define TENSAO_IDEAL  2200


//==================================================================================================
//
//==================================================================================================
void Voltmeter_Init(void)
{
	AD_Init();
}

int16_t Voltmeter_GetVoltage(void)
{
	return AD_GetValue(AD_VOLTMETER);
}


void Voltmeter_Task(void)
{
	/*uint16_t i;
	if(acont==0)
	{
		for(i=0;i<100;i++)
		{
			AD_Volt_Vector[i]=AD_GetValue(AD_VOLTMETER);
		}
		acont=1;
	}
	//return *AD_Volt_Vector;*/
}

//==================================================================================================
// Calcula a tensao da rede
//==================================================================================================
/*
void Voltage_Converter(void)
{
	int8_t i;
	uint16_t uiTemp;

	static uint16_t Acc_Volts;
	static uint16_t Somador_Tensao[4];

	// isso parece ser mais rápido do que um "for" e gasta menos memoria
	Somador_Tensao[3] = Somador_Tensao[2];
	Somador_Tensao[2] = Somador_Tensao[1];
	Somador_Tensao[1] = Somador_Tensao[0];

	Somador_Tensao[0] = (uiVetorTensao*1.38) - 220;

	if(Somador_Tensao[3] == 0) // se ainda  não encheu o FIFO
	{
		Acc_Volts = uiTensao = TENSAO_IDEAL;
	}
	else
	{
		uiTemp =0;

		for(i=0; i<=3; i++)
		{
			uiTemp += Somador_Tensao[i];
		}

		uiTemp = uiTemp/4;

		Acc_Volts = (uiTemp+Acc_Volts)/2;
	}

	if(Acc_Volts > uiTensao)//l, g
	{
		uiTensao++;//g
	}
	else if(Acc_Volts < uiTensao)
	{
		uiTensao--;//g
	}

	if((Acc_Volts-50) > uiTensao)
	{
		uiTensao=Acc_Volts;
	}

	if((Acc_Volts+50) < uiTensao)
	{
		uiTensao=Acc_Volts;
	}

	uiVetorTensao = 0;
}
*/

//==================================================================================================
//
//==================================================================================================
/*
void Liga_Leitura_Tensao(void)
{
	xSemaforoAD = _xBUZY_;

	ADCON1 = 0xE3;  //  Habilitacao da referencia interna 4.096 V

	AD_CHANNEL_REGISTER = 0;  // CANAL DA leitura de tensao

	ucContadorAmostrasTensao = 0;

	uiVetorTensao = 0;
}
*/

/*
void Desliga_Leitura_Tensao(void)
{
	ADCON1 = 0xE0;  //  Referencia interna 4.096V desligada

	xSemaforoAD = _xFREE_;

	AD_CHANNEL_REGISTER = 0;  // CANAL DA leitura de tensao

	blTensaoOk = TRUE;
}
*/

/*
void Nova_Leitura_Tensao(void)
{
	uiVetorTensao +=read_adc();
}
*/

/*
void Leitura_Tensao(void)
{
	DESLIGA_INT_AD;
}
*/

//==================================================================================================
//
//==================================================================================================
/*uint16_t Voltmeter_AD_GetValue(void)
{
	static volatile uint16_t value;
	value += AD_GetValue(AD_VOLTMETER);
	return value;
}*/

/*
void Voltmeter_AD_GetValue(void)
{
	uiVetorTensao += AD_GetValue(AD_VOLTMETER);
}*/
