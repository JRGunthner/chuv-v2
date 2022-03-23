#ifndef __DISPLAY_CHARS_H
#define __DISPLAY_CHARS_H

#define	_SP_		0x00
#define	_APAGADO_	0x00

#define	_SEG_A_	0x01
#define	_SEG_B_	0x02
#define	_SEG_C_	0x04
#define	_SEG_D_	0x08
#define	_SEG_E_	0x10
#define	_SEG_F_	0x20
#define	_SEG_G_	0x40
#define	_PONTO_	0x80

#define	_0_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_)
#define	_1_	(_SEG_B_ | _SEG_C_)
#define	_2_	(_SEG_A_ | _SEG_B_ | _SEG_D_ | _SEG_E_ | _SEG_G_)
#define	_3_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_G_)
#define	_4_	(_SEG_B_ | _SEG_C_ | _SEG_F_ | _SEG_G_)
#define	_5_	(_SEG_A_ | _SEG_C_ | _SEG_D_ | _SEG_F_ | _SEG_G_)
#define	_6_	(_SEG_A_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_7_	(_SEG_A_ | _SEG_B_ | _SEG_C_)
#define	_8_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_9_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_F_ | _SEG_G_)
#define	_A_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_b_	(_SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_c_	(_SEG_D_ | _SEG_E_ | _SEG_G_)
#define	_C_	(_SEG_A_ | _SEG_D_ | _SEG_E_ | _SEG_F_)
#define	_d_	(_SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_G_)
#define	_E_	(_SEG_A_ | _SEG_D_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_F_	(_SEG_A_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_G_	(_SEG_A_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_)
#define	_H_	(_SEG_B_ | _SEG_C_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_h_	(_SEG_C_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_i_	(_SEG_C_)
#define	_I_	(_SEG_B_ | _SEG_C_)
#define	_J_	(_SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_E_)
#define	_L_	(_SEG_D_ | _SEG_E_ | _SEG_F_)
#define	_n_	(_SEG_C_ | _SEG_E_ | _SEG_G_)
#define	_M_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_E_ | _SEG_F_)
#define	_O_	(_SEG_A_ | _SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_)
#define	_o_	(_SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_G_)
#define	_P_	(_SEG_A_ | _SEG_B_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_r_	(_SEG_E_ | _SEG_G_)
#define	_S_	(_SEG_A_ | _SEG_C_ | _SEG_D_ | _SEG_F_ | _SEG_G_)
#define	_t_	(_SEG_D_ | _SEG_E_ | _SEG_F_ | _SEG_G_)
#define	_U_	(_SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_F_)
#define	_u_	(_SEG_C_ | _SEG_D_ | _SEG_E_)
#define _Y_	(_SEG_B_ | _SEG_C_ | _SEG_D_ | _SEG_F_ | _SEG_G_)

#define	_GRAUS_		(_SEG_A_ | _SEG_B_ | _SEG_F_ | _SEG_G_)
#define	_HORAS_		(_h_)
#define	_MINUTOS_	(_SEG_F_)
#define	_SEGUNDOS_	(_SEG_B_ | _SEG_F_)
#define	_POR_CENTO_	(_SEG_C_ | _SEG_D_ | _SEG_E_ | _SEG_G_)
#define	_VOLTS_		(_u_)

uint8_t NUMBER_TABLE[0x11] = {_0_,_1_,_2_,_3_,_4_,_5_,_6_,_7_,_8_,_9_,_A_,_b_,_C_,_d_,_E_,_F_,_SEG_D_};

#endif//__DISPLAY_CHARS_H
