#ifndef __BLUETOOTH_COMMANDS_H
#define __BLUETOOTH_COMMANDS_H

//-------------------------------
// Commands ID - sent by MCU
#define BT_CMD_MAKE_CALL 						0x00 /* This command is used to trigger HF action for making an outgoing call */
#define BT_CMD_MAKE_EXTENSION_CALL 				0x01 /* This command is used to trigger HF action for making an extension call number */
#define BT_CMD_MMI_ACTION 						0x02 /* MMI action */
#define BT_CMD_EVENT_MASK_SETTING 				0x03 /* This command is used to set the specific event mask that host MCU would not like to receive it from BTM */
#define BT_CMD_MUSIC_CONTROL 					0x04 /* This command is used to trigger AVRCP command for music control */
#define BT_CMD_CHANGE_DEVICE_NAME 				0x05
#define BT_CMD_CHANGE_PIN_CODE 					0x06
#define BT_CMD_BTM_PARAMETER_SETTING 			0x07
#define BT_CMD_READ_BTM_VERSION 				0x08
#define BT_CMD_GET_PB_BY_AT_CMD 				0x09
#define BT_CMD_VENDOR_AT_COMMAND 				0x0A
#define BT_CMD_AVC_SPECIFIC_CMD 				0x0B
#define BT_CMD_AVC_GROUP_NAVIGATION 			0x0C
#define BT_CMD_READ_LINK_STATUS 				0x0D
#define BT_CMD_READ_PAIRED_DEVICE_RECORD 		0x0E
#define BT_CMD_READ_LOCAL_BD_ADDRESS 			0x0F
#define BT_CMD_READ_LOCAL_DEVICE_NAME 			0x10
#define BT_CMD_SET_ACCESS_PB_METHOD 			0x11
#define BT_CMD_SEND_SPP_IAP_DATA 				0x12
#define BT_CMD_BTM_UTILITY_FUNCTION 			0x13 /* This command is used to indicate BTM to execute the specific utility function */
#define BT_CMD_ACK 								0x14
#define BT_CMD_ADDITIONAL_PROFILES_LINK_SETUP	0X15
#define BT_CMD_READ_LINKED_DEVICE_INFORMATION	0x16
#define BT_CMD_PROFILES_LINK_BACK 				0x17
#define BT_CMD_DISCONNECT 						0x18
#define BT_CMD_MCU_STATUS_INDICATION 			0x19
#define BT_CMD_USER_CONFIRM_SPP_REQ_REPLY 		0x1A
#define BT_CMD_SET_HF_GAIN_LEVEL 				0x1B
#define BT_CMD_EQ_MODE_SETTING 					0x1C
#define BT_CMD_DSP_NR_CTRL 						0x1D
#define BT_CMD_GPIO_CONTROL 					0x1E
#define BT_CMD_MCU_UART_RX_BUFFER_SIZE 			0x1F
#define BT_CMD_VOICE_PROMPT_CMD 				0x20
#define BT_CMD_MAP_REQUEST 						0x21
#define BT_CMD_SECURITY_BONDING_REQ 			0x22
#define BT_CMD_SET_OVERALL_GAIN 				0x23

//-------------------------------
// Commands PARAMETERS - sent by MCU

/* Parameters of all Command ID */
#define BT_CMD_PARM_DATA_BASE_INDEX_0	0x00
#define BT_CMD_PARM_DATA_BASE_INDEX_1	0x01

/* Parameters of @BT_CMD_MUSIC_CONTROL Command ID */
#define BT_CMD_PARM_STOP_FOR_REW		0x00	/* Stop fast forward or rewind */
#define BT_CMD_PARM_FORWARD				0x01	/* Fast forward */
#define BT_CMD_PARM_FORWARD_REPEAT		0x02	/* Fast forward with repeat send fast forward command every 800ms */
#define BT_CMD_PARM_REWIND				0x03	/* Rewind */
#define BT_CMD_PARM_REWIND_REPEAT		0x04	/* Rewind with repeat send rewind command every 800ms */
#define BT_CMD_PARM_PLAY				0x05	/* PLAY command */
#define BT_CMD_PARM_PAUSE				0x06	/* PAUSE command */
#define BT_CMD_PARM_PLAY_PAUSE			0x07	/* PLAY PAUSE toggle */
#define BT_CMD_PARM_STOP				0x08	/* STOP command */

/* Parameters of @BT_CMD_BTM_UTILITY_FUNCTION Command ID */
#define BT_CMD_PARM_MCU_TO_BTM_NFC		0x00	/* Host MCU ask BTM to process NFC detected function */
#define BT_CMD_PARM_AUX_LINE_DETECT		0x01	/* Notice BTM the aux line in is detected. BTM will process build-in aux line-in detection procedure */
#define BT_CMD_PARM_GENERATE_TONE		0x02	/* Ask BTM to generate the specific tone */ // TODO: substituir o Buzzer por uma função baseada neste comando
#define BT_CMD_PARM_FORCE_NONCONNECT	0x03	/* Forced nonconnectable setting */
#define BT_CMD_PARM_TTS_INDICATION		0x05	/* External TTS indication */

//-------------------------------
// Tone type
/* Second Parameters of @BT_CMD_BTM_UTILITY_FUNCTION Command ID after @BT_CMD_PARM_GENERATE_TONE */
#define BT_TONE_0			0x00	/* N/A - 0Hz, 0s */
#define BT_TONE_1x_200HZ_01	0x01	/* 200Hz,	100ms */
#define BT_TONE_1x_500HZ_01	0x02	/* 500Hz,	100ms */
#define BT_TONE_1x_1KHZ_01	0x03	/* 1kHz,	100ms */
#define BT_TONE_1x_1K5HZ_01	0x04	/* 1.5kHz,	100ms */
#define BT_TONE_1x_2KHH_01	0x05	/* 2kHz,	100ms */
#define BT_TONE_1x_200HZ_1	0x06	/* 200Hz,	1s */
#define BT_TONE_1x_500HZ_1	0x07	/* 500Hz,	1s */
#define BT_TONE_1x_1KHZ_1	0x08	/* 1kHz,	1s */
#define BT_TONE_1x_1K5HZ_1	0x09	/* 1.5kHz,	1s */
#define BT_TONE_1x_2KHZ_1	0x0A	/* 2KHz, 	1s */
#define BT_TONE_2X_200HZ_01	0x0B	/* 200Hz  / mute / 200Hz  - 100ms for each tone */
#define BT_TONE_2X_500HZ_01	0x0C	/* 500Hz  / mute / 500Hz  - 100ms for each tone */
#define BT_TONE_2X_1KHZ_01	0x0D	/* 1KHz   / mute / 1KHz   - 100ms for each tone */
#define BT_TONE_2X_1K5HZ_01	0x0E	/* 1.5kHz / mute / 1.5kHz - 100ms for each tone */
#define BT_TONE_2X_2KHZ_01	0x0F	/* 2kHz   / mute / 2kHz   - 100ms for each tone */
#define BT_TONE_3X_200HZ_01	0x10	/* 200Hz  / mute / 200Hz  / mute / 200Hz  - 100ms for each tone */
#define BT_TONE_3X_500HZ_01	0x11	/* 500Hz  / mute / 500Hz  / mute / 500Hz  - 100ms for each tone */
#define BT_TONE_3X_1KHZ_01	0x12	/* 1kHz   / mute / 1kHz   / mute / 1kHz   - 100ms for each tone */
#define BT_TONE_3X_1K5HZ_01	0x13	/* 1.5kHz / mute / 1.5kHz / mute / 1.5kHz - 100ms for each tone */
#define BT_TONE_3X_2KHZ_01	0x14	/* 2kHz   / mute / 2kHz   / mute / 2kHz   - 100ms for each tone */
#define BT_TONE_4X_200HZ_01	0x15	/* 200Hz  / mute / 200Hz  / mute / 200Hz  / mute / 200Hz  - 100ms for each tone */
#define BT_TONE_4X_500HZ_01	0x16	/* 500Hz  / mute / 500Hz  / mute / 500Hz  / mute / 500Hz  - 100ms for each tone */
#define BT_TONE_4X_1KHZ_01	0x17	/* 1kHz   / mute / 1kHz   / mute / 1kHz   / mute / 1kHz   - 100ms for each tone */
#define BT_TONE_4X_1K5HZ_01	0x18	/* 1.5kHz / mute / 1.5kHz / mute / 1.5kHz / mute / 1.5kHz - 100ms for each tone */
#define BT_TONE_4X_2KHZ_01	0x19	/* 2kHz   / mute / 2kHz   / mute / 2kHz   / mute / 2kHz   - 100ms for each tone */
#define BT_TONE_500_400_300_200_005		0x1A /* 500Hz  / 400Hz/ 300Hz  / 200Hz - 50ms for each tone */
#define BT_TONE_200_300_400_500_005		0x1B /* 200Hz  / 300Hz/ 400Hz  / 500Hz - 50ms for each tone */
#define BT_TONE_400_300_015				0x1C	/* 400Hz  / 300Hz - 150ms for each tone */
#define BT_TONE_300_400_015				0x1D	/* 300Hz  / 400Hz - 150ms for each tone */
#define BT_TONE_300_0_400_0_500_0_1K_01	0x1E	/* 300Hz  / mute / 400Hz  / mute / 500Hz  / mute / 1000Hz - 100ms for each tone */
#define BT_TONE_1K_0_500_0_400_0_300_01	0x1F	/* 1kHz   / mute / 500Hz  / mute / 400Hz  / mute / 300Hz  - 100ms for each tone */
#define BT_TONE_MELODY_1	0x20	/* ROM build-in multi tone melody */
#define BT_TONE_MELODY_2	0x21	/* ROM build-in multi tone melody */
#define BT_TONE_MELODY_3	0x22	/* ROM build-in multi tone melody */
#define BT_TONE_MELODY_4	0x23	/* ROM build-in multi tone melody */
#define BT_TONE_MELODY_5	0x24	/* ROM build-in multi tone melody */
#define BT_TONE_MELODY_6	0x25	/* ROM build-in multi tone melody */
#define BT_TONE_MELODY_7	0x26	/* ROM build-in multi tone melody */


//-------------------------------
// Events ID - sent by Bluetooth Module
#define BT_EVENT_ACK									0x00
#define BT_EVENT_BTM_STATUS								0x01
#define BT_EVENT_CALL_STATUS							0x02
#define BT_EVENT_CALLER_ID								0x03
#define BT_EVENT_SMS_RECEIVED_INDICATION				0x04
#define BT_EVENT_MISSED_CALL_INDICATION 				0x05
#define BT_EVENT_PHONE_MAX_BATTERY_LEVEL 				0x06
#define BT_EVENT_PHONE_CURRENT_BATTERY_LEVEL			0x07
#define BT_EVENT_ROAMING_STATUS							0x08
#define BT_EVENT_PHONE_MAX_SIGNAL_STRENGTH_LEVEL		0x09
#define BT_EVENT_PHONE_CURRENT_SIGNAL_STRENGTH_LEVEL	0x0A
#define BT_EVENT_PHONE_SERVICE_STATUS					0x0B
#define BT_EVENT_BTM_BATTERY_STATUS						0x0C
#define BT_EVENT_BTM_CHARGING_STATUS					0x0D
#define BT_EVENT_RESET_TO_DEFAULT						0x0E
#define BT_EVENT_REPORT_HF_GAIN_LEVEL					0x0F
#define BT_EVENT_EQ_MODE_INDICATION						0x10
#define BT_EVENT_PBAP_MISSED_CALL_HISTORY				0x11
#define BT_EVENT_PBAP_RECEIVED_CALL_HISTORY				0x12
#define BT_EVENT_PBAP_DIALED_CALL_HISTORY				0x13
#define BT_EVENT_PBAP_COMBINE_CALL_HISTORY				0x14
#define BT_EVENT_PHONEBOOK_CONTACTS						0x15
#define BT_EVENT_PBAP_ACCESS_FINISH						0x16
#define BT_EVENT_READ_LINKED_DEVICE_INFORMATION_REPLY	0x17
#define BT_EVENT_READ_BTM_VERSION_REPLY					0x18
#define BT_EVENT_CALL_LIST_REPORT						0x19
#define BT_EVENT_AVC_SPECIFIC_RSP 						0x1A
#define BT_EVENT_BTM_UTILITY_REQ 						0x1B
#define BT_EVENT_VENDOR_AT_CMD_REPLY 					0x1C
#define BT_EVENT_REPORT_VENDOR_AT_EVENT 				0x1D
#define BT_EVENT_READ_LINK_STATUS_REPLY 				0x1E
#define BT_EVENT_READ_PAIRED_DEVICE_RECORD_REPLY		0X1F
#define BT_EVENT_READ_LOCAL_BD_ADDRESS_REPLY 			0x20
#define BT_EVENT_READ_LOCAL_DEVICE_NAME_REPLY 			0x21
#define BT_EVENT_REPORT_SPP_IAP_DATA 					0x22
#define BT_EVENT_REPORT_LINK_BACK_STATUS 				0x23
#define BT_EVENT_RINGTONE_FINISH_INDICATE 				0x24
#define BT_EVENT_USER_CONFIRM_SSP_REQ 					0x25
#define BT_EVENT_REPORT_AVRCP_VOL_CTRL 					0x26
#define BT_EVENT_REPORT_INPUT_SIGNAL_LEVEL 				0x27
#define BT_EVENT_REPORT_IAP_INFO 						0x28
#define BT_EVENT_REPORT_AVRCP_ABS_VOL_CTRL 				0x29
#define BT_EVENT_REPORT_VOICE_PROMPT_STATUS 			0x2A
#define BT_EVENT_REPORT_MAP_DATA 						0x2B
#define BT_EVENT_SECURITY_BONDING_RES 					0x2C
#define BT_EVENT_REPORT_TYPE_CODEC 						0x2D

#endif /*__BLUETOOTH_COMMANDS_H */
