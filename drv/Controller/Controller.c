#include "Controller.h"
#include "Flowmeter.h"
#include "NTC.h"

#define DIVISOR_DERIVADA 8

#define ZM 3									// Zona morta
#define KP 25   						       	// Constante proporcional
#define KI 2 								   	// Constante Integral
#define KD 100   							   	// Constante Derivativa
#define LS (uint16_t)(ucMaximoPID-ucOffsetPID) 	// Limite superior do PID
#define LI 0     							    // Limite Inferior do PID

uint16_t TemperatureSetPoint;

uint16_t ucOffsetPID;
uint16_t ucMaximoPID;

int16_t  DerivadaFluxo;
uint16_t TemperaturaEntrada;
uint16_t TemperaturaSaida;
int16_t  FluxoAgua;

int16_t Soma_PID;
int16_t Prop;
int16_t Derivativo;
int16_t Integral;

int16_t Calculo_Integral;
int16_t Calculo_Proporcional;
int16_t Calculo_Derivativo;
int16_t Saida_PID;

uint16_t uiPower;

//==================================================================================================
//
//==================================================================================================
uint16_t Controller_GetSetPoint()
{
	return TemperatureSetPoint;
}

//==================================================================================================
// Funcao que reseta os calculos
//==================================================================================================
void Controller_ResetPID(void)
{
	Saida_PID = 0;
	Soma_PID  = 0;
	Integral  = 0;

	Calculo_Derivativo 	 = 0;
	Calculo_Integral 	 = 0;
	Calculo_Proporcional = 0;
}

//==================================================================================================
//
//==================================================================================================
void Calculo_PID(int16_t Erro_Pid)
{
	static int16_t Ultimo_Erro;
	int16_t acc;

	//--------------------------------------
	//         PROPORCIONAL
	//--------------------------------------
	Prop = (int16_t)((int16_t)KP * (int16_t)Erro_Pid); // Ajustado para ponto fixo

	if (Prop > LS*100)// 10000 e' o mesmo que 100 pelo ponto fixo
	{
		Prop = LS*100;
	}
	if(Prop < 0)
	{
		Prop = 0;
	}

	Calculo_Proporcional = (signed char) (Prop/100);// resultado devolve para inteiro

	//--------------------------------------
	//         INTEGRAL
	//--------------------------------------
	Integral = Integral + (int16_t)((int16_t)Erro_Pid*(int16_t)KI);

	acc = Integral + Prop;

	if (acc > LS*100)
	{
		Integral = LS*100 - Prop;
	}
	else if (acc < 0)
	{
		Integral = 0;
	}

	Calculo_Integral = (signed char)(Integral/100);

	//--------------------------------------
	//         DERIVATIVO
	//--------------------------------------
	if ((Erro_Pid - Ultimo_Erro) != 0)
	{
		Derivativo = (int16_t)((int16_t)( Erro_Pid - Ultimo_Erro )*(int16_t)KD);
	}
	else
	{
		Derivativo = 0;
	}

	Ultimo_Erro = Erro_Pid;

	Calculo_Derivativo = (signed char)(Derivativo/100);

	//--------------------------------------
	//            SOMATORIO
	//--------------------------------------
	Soma_PID = (Prop + Integral + Derivativo);

	Soma_PID /= 100;

	if((int16_t)Soma_PID > LS)
	{
		Saida_PID = LS;
	}
	else if((int16_t)Soma_PID < LI)
	{
		Saida_PID = LI;
	}
	else
	{
		Saida_PID =(unsigned char)Soma_PID;
	}
}

//==================================================================================================
//
//==================================================================================================
void PID_Task(void)
{
    int16_t erro;

	erro = (TemperatureSetPoint) - TemperaturaSaida;

	if((erro < -ZM) || (erro > ZM))
	{
		Calculo_PID(erro);
	}
	else
	{
		Calculo_PID(0);
	}
}

//==================================================================================================
// calcula o erro e escolhe a melhor maneira do funcionamento por temperatura
//==================================================================================================
void Controle_Modo_Temperatura()
{
    int16_t Acc;
    uint16_t Bcc;

    Acc = (TemperatureSetPoint) - TemperaturaSaida;

    if (Acc > -15)
    {
        Bcc = Saida_PID + ucOffsetPID + (DerivadaFluxo/DIVISOR_DERIVADA);
    }
    else if ((Acc > -45))
    {
        Bcc = ucOffsetPID + (DerivadaFluxo/DIVISOR_DERIVADA);
    }
    else
    {
        Bcc = 0;
        Controller_ResetPID();
    }

    if (Bcc > ucMaximoPID)
    {
        Bcc = ucMaximoPID;
    }

    if (Bcc > 100)
    {
        Bcc = 100;
    }

    uiPower = Bcc;
}

//==================================================================================================
//
//==================================================================================================
void Calcula_Offset_PID()
{
    static int16_t Delta;
    static int32_t Acc;

    Delta = (TemperatureSetPoint - TemperaturaEntrada);

    if(Delta >= 10)
    {
        Acc = (int32_t)((int32_t)FluxoAgua*(int32_t)Delta);

		#if  (defined _127V)
			Delta = (Acc*0.00103) + 6.5;
		#else
			Delta = (Acc*0.00065) + 7.5;
		#endif

        if (Delta > 100)
        {
            Delta = 100 ;
        }

        if (Delta < 0)
        {
            Delta = 0 ;
        }

        Delta = ucOffsetPID  + Delta;

        ucOffsetPID = Delta/2 ;

        if(ucOffsetPID > 80)
        {
            ucOffsetPID = 80;
            ucMaximoPID = 100;
        }
        else
        {
            ucMaximoPID = (ucOffsetPID*1.5) + 18;

            if(ucMaximoPID> 100)
            {
                ucMaximoPID = 100;
            }
        }
    }
    else
    {
        ucMaximoPID = 0;
        ucOffsetPID = 0;
    }
}

//==================================================================================================
//
//==================================================================================================
uint16_t Controller_Task(uint16_t SetPoint)
{
	TemperatureSetPoint = SetPoint;
    TemperaturaEntrada  = NTC_GetWaterTemperature(TEMPERATURE_INLET);
    TemperaturaSaida    = NTC_GetWaterTemperature(TEMPERATURE_OUTLET);
    FluxoAgua           = Flowmeter_GetFlow();
    DerivadaFluxo       = Flowmeter_GetFlowDerivative();

    //PID_Task();
    Calcula_Offset_PID();
    Controle_Modo_Temperatura();

    return uiPower;
}
