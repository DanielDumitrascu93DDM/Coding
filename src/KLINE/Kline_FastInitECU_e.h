/*
 * DDM 01/10/2019
 *  */

#ifndef _KLINE_KLINE_FASTINITECU_E_H_
#define _KLINE_KLINE_FASTINITECU_E_H_

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "Kline_t.h"

extern void PORT_g_fvKlineECUInit        ( void );
extern void PORT_g_fvKlineECUHandler     ( void );
PORT_tenAPPReturns PORT_g_fvKlineECURX   ( uint8 u8DataByte );
extern void PORT_g_fvKlineECUStop	 ( void );

#endif // _KLINE_KLINE_FASTINITECU_E_H_
/******************************************************************************
 * EOF: Kline_FastInitECU_e.h
 *******************************************************************************/
