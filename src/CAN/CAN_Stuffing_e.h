/*
 * DDM 27/09/2019
 *  */

#ifndef _CAN_CAN_STUFFING_E_H_
#define _CAN_CAN_STUFFING_E_H_

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "CAN_Stuffing_t.h"

extern void PORT_g_fvCANStuffingInit        ( void );
extern void PORT_g_fvCANStuffingHandler     ( void );
extern void PORT_g_fvCANStuffingSTOP        ( void );
extern void PORT_g_fvCANDestuffingInit      ( void );
extern void PORT_g_fvCANDestuffingHandler   ( void );
extern void PORT_g_fvCANDestuffingStop      ( void );

#endif // _CAN_CAN_STUFFING_E_H_
/******************************************************************************
 * EOF: CAN_Stuffing_e.h
 *******************************************************************************/
