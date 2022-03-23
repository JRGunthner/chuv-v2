#ifndef __TRANSLATEING_H
#define __TRANSLATEING_H

#define SHORTMSG_INLET_TEMPERATURE    "IT"
#define SHORTMSG_OUTLET_TEMPERATURE   "OT"
#define SHORTMSG_LITERS               "LT"
#define SHORTMSG_FLOW                 "FL"
#define SHORTMSG_SHOWER_TIME		"BT"
#define SHORTMSG_LOG                  "LG"
#define SHORTMSG_MONTH_COMSUMPTION    "BC"
#define SHORTMSG_NOT_APPLIED          "NA"

#define MSG_LED_OFF              " Led Off "
#define MSG_LED_OFF_SIZE         9

#define MSG_LED_ON               " Led On "
#define MSG_LED_ON_SIZE          8

#define MSG_LED_TEMPERATURE      " Led Temperature "
#define MSG_LED_TEMPERATURE_SIZE 17

#define MSG_LED_AUTO             " Led Auto "
#define MSG_LED_AUTO_SIZE        10

#define MSG_LED_FIXED            " Led Fixed "
#define MSG_LED_FIXED_SIZE       11

#define MSG_LED_POWER            " Led Power "
#define MSG_LED_POWER_SIZE       11

#define MSG_BIP_ON               " Bip On "
#define MSG_BIP_ON_SIZE          8

#define MSG_BIP_OFF              " Bip Off "
#define MSG_BIP_OFF_SIZE         9

#define MSG_MYSHOWER             "  MyShower  "
#define MSG_MYSHOWER_SIZE        12

#define MSG_SHOWERSHOW           "  ShowerShow  "
#define MSG_SHOWERSHOW_SIZE      14

#ifdef SHOWER_SHOW
	#define MSG_WELLCOME		MSG_SHOWERSHOW
	#define MSG_WELLCOME_SIZE	MSG_SHOWERSHOW_SIZE
#else
	#define MSG_WELLCOME		MSG_MYSHOWER
	#define MSG_WELLCOME_SIZE	MSG_MYSHOWER_SIZE
#endif

#define MSG_LITERS               " Liters "
#define MSG_LITERS_SIZE          8

#define MSG_PASSWORD             " Password "
#define MSG_PASSWORD_SIZE        10

#define MSG_ERROR                " ERROR "
#define MSG_ERROR_SIZE           7

#endif /* __TRANSLATEING_H */
