/*
 * DDM 01/10/2019
 *  */

#ifndef _KLINE_KLINE_FASTINITTESTER_E_H_
#define _KLINE_KLINE_FASTINITTESTER_E_H_

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "Kline_t.h"

extern void PORT_g_fvKlineTesterInit        ( void );
extern void PORT_g_fvKlineTesterHandler     ( void );
PORT_tenAPPReturns PORT_g_fvKlineTesterRX   ( uint8 u8DataByte );
extern void PORT_g_fvKlineTesterStop	    ( void );

#endif // _KLINE_KLINE_FASTINITTESTER_E_H_
/******************************************************************************
 * EOF: Kline_FastInitTester_e.h
 *******************************************************************************/
