#include "Display.h"
#include "Display_Chars.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

#define DISPLAY_DIGIT_LEFT  1
#define DISPLAY_DIGIT_RIGHT 0

#define DISP_A	 GPIO_Pin_2  // PB2
#define DISP_B	 GPIO_Pin_13 // PB13
#define DISP_C	 GPIO_Pin_14 // PB14
#define DISP_D	 GPIO_Pin_12 // PB12
#define DISP_E	 GPIO_Pin_10 // PB10
#define DISP_F	 GPIO_Pin_11 // PB11
#define DISP_G	 GPIO_Pin_12 // PA12
#define DISP_DOT GPIO_Pin_6  // PF6
#define DISP_UN	 GPIO_Pin_7  // PF7

uint8_t DigitRight;
uint8_t DigitLeft;

uint8_t PointMiddle;
uint8_t PointRight;

//==================================================================================================
//
//==================================================================================================
void Display_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

	/* A, B, C, D, E, F segments */
	GPIO_InitStructure.GPIO_Pin   = DISP_A|DISP_B|DISP_C|DISP_D|DISP_E|DISP_F;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* DOT segment and Display Select */
	GPIO_InitStructure.GPIO_Pin   = DISP_DOT|DISP_UN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* G segment */
	GPIO_InitStructure.GPIO_Pin   = DISP_G;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Clear all display pins
	GPIO_WriteBit(GPIOB, DISP_A,0);
	GPIO_WriteBit(GPIOB, DISP_B,0);
	GPIO_WriteBit(GPIOB, DISP_C,0);
	GPIO_WriteBit(GPIOB, DISP_D,0);
	GPIO_WriteBit(GPIOB, DISP_E,0);
	GPIO_WriteBit(GPIOB, DISP_F,0);
	GPIO_WriteBit(GPIOA, DISP_G,0);
	GPIO_WriteBit(GPIOF, DISP_DOT,0);

	DigitRight = 0x00;
	DigitLeft  = 0x00;

	PointMiddle = 0;
	PointRight  = 0;
}

//==================================================================================================
// Update Display
//==================================================================================================
void Display_ParallelInterface(uint8_t ucByte)
{
	union
	{
		uint8_t _Byte;
		struct
		{
			uint8_t _bA :1;
			uint8_t _bB :1;
			uint8_t _bC :1;
			uint8_t _bD :1;
			uint8_t _bE :1;
			uint8_t _bF :1;
			uint8_t _bG :1;
			uint8_t _bDP:1;
		}_Bits;
	}Scc;

	Scc._Byte = ucByte;
	GPIO_WriteBit(GPIOB, DISP_A,  (BitAction)Scc._Bits._bA);
	GPIO_WriteBit(GPIOB, DISP_B,  (BitAction)Scc._Bits._bB);
	GPIO_WriteBit(GPIOB, DISP_C,  (BitAction)Scc._Bits._bC);
	GPIO_WriteBit(GPIOB, DISP_D,  (BitAction)Scc._Bits._bD);
	GPIO_WriteBit(GPIOB, DISP_E,  (BitAction)Scc._Bits._bE);
	GPIO_WriteBit(GPIOB, DISP_F,  (BitAction)Scc._Bits._bF);
	GPIO_WriteBit(GPIOA, DISP_G,  (BitAction)Scc._Bits._bG);
	GPIO_WriteBit(GPIOF, DISP_DOT,(BitAction)Scc._Bits._bDP);

}

//==================================================================================================
// Char table
//==================================================================================================
uint8_t Display_ASCIITable(uint8_t ch)
{
	uint8_t Return_Char;

	switch(ch)
	{
		//----------------------------
		// Numbers
		case '0': {Return_Char = _0_; break;}
		case '1': {Return_Char = _1_; break;}
		case '2': {Return_Char = _2_; break;}
		case '3': {Return_Char = _3_; break;}
		case '4': {Return_Char = _4_; break;}
		case '5': {Return_Char = _5_; break;}
		case '6': {Return_Char = _6_; break;}
		case '7': {Return_Char = _7_; break;}
		case '8': {Return_Char = _8_; break;}
		case '9': {Return_Char = _9_; break;}

		//----------------------------
		// Letters
		case 'A': {Return_Char = _A_; break;}
		case 'a': {Return_Char = _A_; break;}
		case 'B': {Return_Char = _b_; break;}
		case 'b': {Return_Char = _b_; break;}
		case 'C': {Return_Char = _C_; break;}
		case 'c': {Return_Char = _c_; break;}
		case 'D': {Return_Char = _d_; break;}
		case 'd': {Return_Char = _d_; break;}
		case 'E': {Return_Char = _E_; break;}
		case 'e': {Return_Char = _E_; break;}
		case 'F': {Return_Char = _F_; break;}
		case 'f': {Return_Char = _F_; break;}
		case 'G': {Return_Char = _G_; break;}
		case 'g': {Return_Char = _G_; break;}
		case 'H': {Return_Char = _H_; break;}
		case 'h': {Return_Char = _h_; break;}
		case 'i': {Return_Char = _i_; break;}
		case 'I': {Return_Char = _I_; break;}
		case 'J': {Return_Char = _J_; break;}
		case 'j': {Return_Char = _J_; break;}
		//case 'K': {Return_Char = ???; break;}
		//case 'k': {Return_Char = ???; break;}
		case 'L': {Return_Char = _L_; break;}
		case 'l': {Return_Char = _L_; break;}
		case 'M': {Return_Char = _M_; break;}
		case 'm': {Return_Char = _M_; break;}
		case 'N': {Return_Char = _n_; break;}
		case 'n': {Return_Char = _n_; break;}
		case 'O': {Return_Char = _O_; break;}
		case 'o': {Return_Char = _o_; break;}
		case 'P': {Return_Char = _P_; break;}
		case 'p': {Return_Char = _P_; break;}
		//case 'Q': {Return_Char = ???; break;}
		//case 'q': {Return_Char = ???; break;}
		case 'R': {Return_Char = _r_; break;}
		case 'r': {Return_Char = _r_; break;}
		case 'S': {Return_Char = _S_; break;}
		case 's': {Return_Char = _S_; break;}
		case 'T': {Return_Char = _t_; break;}
		case 't': {Return_Char = _t_; break;}
		case 'U': {Return_Char = _u_; break;}
		case 'u': {Return_Char = _u_; break;}
		case 'V': {Return_Char = _U_; break;}
		case 'v': {Return_Char = _U_; break;}
		case 'W': {Return_Char = _u_; break;}
		case 'w': {Return_Char = _u_; break;}
		case 'X': {Return_Char = _H_; break;}
		case 'x': {Return_Char = _H_; break;}
		case 'Y': {Return_Char = _Y_; break;}
		case 'y': {Return_Char = _Y_; break;}
		case 'Z': {Return_Char = _2_; break;}
		case 'z': {Return_Char = _2_; break;}

		//----------------------------
		// Special Characters
		case '�':  {Return_Char = _GRAUS_; 	  break;}
		case '-':  {Return_Char = _SEG_G_; 	  break;}
		case '_':  {Return_Char = _SEG_D_; 	  break;}
		case ' ':  {Return_Char = _SP_;    	  break;}
		case '\'': {Return_Char = _MINUTOS_;  break;}
		case '\"': {Return_Char = _SEGUNDOS_; break;}

		//----------------------------
		// Default
		default:
		{
			Return_Char = 0;
			break;
		}
	}
	return Return_Char;
}

//==================================================================================================
// Updates Displays Char
//==================================================================================================
void Display_UpdateChar(uint8_t CharLeft, uint8_t CharRight)
{
	DigitRight  = Display_ASCIITable(CharRight);
	DigitLeft   = Display_ASCIITable(CharLeft);
	PointMiddle = 0;
	PointRight  = 0;
}

//==================================================================================================
//
//==================================================================================================
void Display_UpdateChar2(char *Char)
{
	DigitRight  = Display_ASCIITable(Char[1]);
	DigitLeft   = Display_ASCIITable(Char[0]);
	PointMiddle = 0;
	PointRight  = 0;
}

//==================================================================================================
//
//==================================================================================================
void Display_UpdatePoints(uint8_t Value, uint8_t Point)
{
	if(Point & DISPLAY_POINT_MIDDLE) PointMiddle = Value;
	if(Point & DISPLAY_POINT_RIGHT ) PointRight  = Value;
}

//==================================================================================================
//
//==================================================================================================
void Display_UpdateDecimal(uint8_t DecimalDigitLeft, uint8_t DecimalDigitRight)
{
	DigitRight = NUMBER_TABLE[DecimalDigitLeft];
	DigitLeft  = NUMBER_TABLE[DecimalDigitRight];
}

//==================================================================================================
//
//==================================================================================================
void Display_Task(void)
{
	static uint8_t DigitSelect;

	if(DigitSelect == DISPLAY_DIGIT_RIGHT)
	{
		Display_ParallelInterface(_SP_);

		/* Enables Digit Right, DISP_UN = 0 */
		GPIO_WriteBit(GPIOF, DISP_UN, (BitAction)DISPLAY_DIGIT_RIGHT);

		if(PointRight)
		{
			Display_ParallelInterface(DigitRight | 0x80);
		}
		else
		{
			Display_ParallelInterface(DigitRight);
		}

		DigitSelect = DISPLAY_DIGIT_LEFT;
	}
	else // DigitSelect != DISPLAY_DIGIT_LEFT)
	{
		Display_ParallelInterface(_SP_);

		/* Enables Digit Left, DISP_UN = 1 */
		GPIO_WriteBit(GPIOF, DISP_UN, (BitAction)DISPLAY_DIGIT_LEFT);

		if(PointMiddle)
		{
			Display_ParallelInterface(DigitLeft | 0x80);
		}
		else
		{
			Display_ParallelInterface(DigitLeft);
		}

		DigitSelect = DISPLAY_DIGIT_RIGHT;
	}
}

//==================================================================================================
//
//==================================================================================================
void Display_UpdateValue(int32_t Value)
{
	if((Value > -19) && (Value < 0)) // -X to -1.0
	{
		DigitRight = NUMBER_TABLE[(~Value+1)/10];
		DigitLeft  = _SEG_G_;

		PointMiddle = 1;
		PointRight  = 0;
	}
	else if((Value >= 0) && (Value < 10))  // 0 to 0.9
	{
		DigitRight = NUMBER_TABLE[Value];
		DigitLeft  = NUMBER_TABLE[0];

		PointMiddle = 1;
		PointRight  = 0;
	}
	else if(Value < 100) // 1.0 to 9.9
	{
		DigitRight = NUMBER_TABLE[Value%10];
		DigitLeft  = NUMBER_TABLE[Value/10];

		PointMiddle = 1;
		PointRight  = 0;
	}
	else if(Value >= 100)  // 10.0 to X
	{
		int32_t val = Value/10;

		DigitRight = NUMBER_TABLE[val%10];
		DigitLeft  = NUMBER_TABLE[val/10];

		PointMiddle = 0;
		PointRight  = 0;
	}
	else
	{
		DigitLeft  = _E_;
		DigitRight = _r_;
	}
}

void Display_UpdateTimeValue(uint32_t Value)
{
	PointMiddle = 0;
	PointRight  = 0;

	if((Value >= 0) && (Value <= 9))  // 0 to 9
	{
		DigitRight = NUMBER_TABLE[Value];
		DigitLeft  = NUMBER_TABLE[0];
	}
	else if((Value >= 10) && (Value <= 99)) // 10 to 99
	{
		DigitRight = NUMBER_TABLE[Value%10];
		DigitLeft  = NUMBER_TABLE[Value/10];
	}
	else if((Value >= 100) && (Value <= 999)) // 100 to 999
	{
		DigitRight = NUMBER_TABLE[(Value/10)%10];
		DigitLeft  = NUMBER_TABLE[(Value/10)/10];
	}
	else if((Value >= 1000) && (Value <= 9999)) // 1000 to 9999
	{
		DigitRight = NUMBER_TABLE[(Value/100)%10];
		DigitLeft  = NUMBER_TABLE[(Value/100)/10];
	}
	else if((Value >= 10000) && (Value <= 99999)) // 10000 to 99999
	{
		DigitRight = NUMBER_TABLE[(Value/1000)%10];
		DigitLeft  = NUMBER_TABLE[(Value/1000)/10];
	}
	else
	{
		DigitLeft  = _E_;
		DigitRight = _r_;
	}
}

//==================================================================================================
// Print firmware version
//==================================================================================================
void Display_Version(uint8_t Major, uint8_t Minor)
{
	PointMiddle = 1;
	PointRight  = 0;

	DigitRight = NUMBER_TABLE[Minor];
	DigitLeft  = NUMBER_TABLE[Major];
}
