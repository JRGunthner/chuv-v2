#include "Bluetooth.h"
#include "Bluetooth_Commands.h"

//==================================================================================================
//
//==================================================================================================
void Bluetooth_Init(void)
{
	USART_InitTypeDef USART_InitStructure;   // USART configuration struct
	GPIO_InitTypeDef  GPIO_InitStructure;    // GPIO configuration struct
	//NVIC_InitTypeDef  NVIC_InitStructure;  // NVIC configuration struct

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Enables High Speed APB (APB2) peripheral clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	/* Connect PB6 to USART1_Tx */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
	/* Connect PB7 to USART1_Rx */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);

	/* Configure USART1 pins:  Rx and Tx ----------------------------*/
	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* USARTx configuration ----------------------------------------------------*/
	/* USARTx configured as follow:
	- BaudRate = 9600 baud
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate 	 = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 	 = USART_StopBits_1;
	USART_InitStructure.USART_Parity 	 = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 		 = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* NVIC configuration */
	/* Enable the USARTx Interrupt */
/*	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	*/

	/* Enable USART */
	USART_Cmd(USART1,ENABLE);
}

//==================================================================================================
//
//==================================================================================================
void Bluetooth_Write(char *Data, uint32_t Tam)
{
	uint32_t i;
	
	for(i=0; i<Tam; i++)
	{	 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		USART_SendData(USART1, Data[i]);
	}
}


//==================================================================================================
// BM20 Protocol
//==================================================================================================
/*
 * 		MCU							BLUETOOTH
 * 		 |								|
 * 		 |								|
 * 		 |	====>>>> COMMAND ====>>>>	|
 * 		 |								|
 * 		 |	<<<<====   ACK   <<<<====	|
 * 		 |								|
 * 		 |				*				|
 * 		 |				*				|
 * 		 |		  BM20 perform 			|
 * 		 |		  the command			|
 * 		 |				*				|
 * 		 |				*				|
 * 		 |								|
 * 		 |	<<<<====  EVENT  <<<<====	|
 * 		 |								|
 * 		 |	====>>>>   ACK   ====>>>>	|
 * 		 |								|
 *
 *
 * ==========================================================
 * 	COMMAND PACKET FORMAT
 * ==========================================================
 *
 *	  Byte	   |	Value	|	Meaning
 *	___________|____________|_____________________________________________________________
 *		0	   |	0xAA	|	Start
 *		1	   |	0xNN	|	Payload length high byte (including command ID and parameters)
 *		2	   |	0xNN	|	Payload length low byte (including command ID and parameters)
 *		3	   |	0xNN 	|	Command ID
 *		4	   |	0xNN	|	Command parameters
 *		N�	   |	0xNN	|	Command parameters
 *	Last byte  |	0xNN	|	CHECKSUM
 *
 *
 * ==========================================================
 * 	EVENT PACKET FORMAT
 * ==========================================================
 *
 *	  Byte	   |	Value	|	Meaning
 *	___________|____________|_____________________________________________________________
 *		0	   |	0xAA	|	Start
 *		1	   |	0xNN	|	Payload length high byte (including event ID and parameters)
 *		2	   |	0xNN	|	Payload length low byte (including event ID and parameters)
 *		3	   |	0xNN 	|	Event ID
 *		4	   |	0xNN	|	Event parameters
 *		N�	   |	0xNN	|	Event parameters
 *	Last byte  |	0xNN	|	CHECKSUM
 *
 *
 * ==========================================================
 * 	CALCULATION CHECKSUM
 * ==========================================================
 *
 *  CHECKSUM = (0xFF + 0x01) - (Byte 1 + Byte 2 + Byte 3 + Byte 4 + Byte N)
 *
 *
 *
 * 			 |\
 * 			 | \		||||||||\	||\		   /||
 * 			 ||\\		||		||	||\\	  //||
 * 		 \\	 || \\		||		||	|| \\	 //	||
 * 		  \\ ||	//		||		||	||	\\	//	||
 * 		   \\||//		||	   //	||	 \\//	||
 * 		   	\||/		||||||||	||	  \/	||
 * 		   	/||\		||		||	||			||
 * 		   //||\\		||		||	||			||
 * 		  // ||	\\		||		||	||			||
 * 		 //	 ||	//		||		||	||			||
 * 			 ||//		||		||	||			||
 * 			 | /		|||||||||	||			||
 * 			 |/
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

