#ifndef __TRANSLATEBR_H
#define __TRANSLATEBR_H

#define SHORTMSG_INLET_TEMPERATURE	"TE" // Temperatura de entrada
#define SHORTMSG_OUTLET_TEMPERATURE	"TS" // Temperatura de saída
#define SHORTMSG_LITERS				"LT" // Litros consumidos
#define SHORTMSG_FLOW				"VZ" // Vazão
#define SHORTMSG_SHOWER_TIME		"TB" // Tempo de banho
#define SHORTMSG_LOG				"LG" // Log de banho
#define SHORTMSG_MONTH_COMSUMPTION	"cB" // Custo mensal de um banho
#define SHORTMSG_NOT_APPLIED		"NA" // Não foi implementado

#define SHORTMSG_PERIOD_ON			"Li" // Tempo ligado
#define SHORTMSG_PERIOD_OFF			"DL" // Tempo desligado
#define SHORTMSG_KILOWATT_COST		"RS" // Custo quilowatt
#define SHORTMSG_PASSWORD			"SE" // Password

#define SHORTMSG_TRIAC_1	"T1"
#define SHORTMSG_TRIAC_2	"T2"
#define SHORTMSG_TRIAC_1_2	"T3"

#define SHORTMSG_ERROR_1	"E1"
#define SHORTMSG_ERROR_2	"E2"
#define SHORTMSG_ERROR_3	"E3"
#define SHORTMSG_ERROR_4	"E4"
#define SHORTMSG_ERROR_5	"E5"

#define MSG_LED_OFF			" Led Off "
#define MSG_LED_OFF_SIZE	9

#define MSG_LED_ON			" Led On "
#define MSG_LED_ON_SIZE		8

#define MSG_LED_TEMPERATURE			" Led Temperatura "
#define MSG_LED_TEMPERATURE_SIZE	17

#define MSG_LED_AUTO			" Led Auto "
#define MSG_LED_AUTO_SIZE		10

#define MSG_LED_FIXED			" Led Fixo "
#define MSG_LED_FIXED_SIZE		11

#define MSG_LED_POWER			" Led Potencia "
#define MSG_LED_POWER_SIZE		14

#define MSG_BIP_ON				" Bip On "
#define MSG_BIP_ON_SIZE			8

#define MSG_BIP_OFF				" Bip Off "
#define MSG_BIP_OFF_SIZE		9

#define MSG_MYSHOWER			"  MyShower  "
#define MSG_MYSHOWER_SIZE		12

#define MSG_SHOWERSHOW			"  ShowerShow  "
#define MSG_SHOWERSHOW_SIZE		14

#ifdef SHOWER_SHOW
	#define MSG_WELLCOME		MSG_SHOWERSHOW
	#define MSG_WELLCOME_SIZE	MSG_SHOWERSHOW_SIZE
#else
	#define MSG_WELLCOME		MSG_MYSHOWER
	#define MSG_WELLCOME_SIZE	MSG_MYSHOWER_SIZE
#endif

#define MSG_LITERS				" Litros "
#define MSG_LITERS_SIZE			8

#define MSG_PASSWORD			" Senha "
#define MSG_PASSWORD_SIZE		7

#define MSG_ERROR				" ERRO "
#define MSG_ERROR_SIZE			6

#define MSG_END_SHOWER			" Fim do Banho "
#define MSG_END_SHOWER_SIZE		14

#define MSG_REDUCE_FLOW			" Reduza a Vazao de Agua "
#define MSG_REDUCE_FLOW_SIZE	24

#define MSG_GRADE				" Nota "
#define MSG_GRADE_SIZE			6

#define MSG_SHOWER_PERIOD		"  Tempo de Banho "
#define MSG_SHOWER_PERIOD_SIZE	17

#define MSG_WATER_CONSUMPTION			"  Consumo de Agua "
#define MSG_WATER_CONSUMPTION_SIZE		18

#define MSG_ENERGY_CONSUMPTION			"  Consumo de Energia "
#define MSG_ENERGY_CONSUMPTION_SIZE		21

#define MSG_ESTIMATED_MONTHLY_COST		"  Custo Estimado Mensal "
#define MSG_ESTIMATED_MONTHLY_COST_SIZE	24

#endif /* __TRANSLATEBR_H */
