/*
 * DDM 01/10/2019
 *  */

#ifndef _CAN_CAN_TP_E_H_
#define _CAN_CAN_TP_E_H_
//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "../ForOthers.h"

#include "CAN_Stuffing_t.h"
#include "CAN_TP_t.h"


extern void PORT_g_fvCANTPInit                      ( void );
extern void PORT_g_fvCANTPHandler                   ( void );
extern PORT_tenAPPReturns PORT_g_fvCANTPSendMessage ( PORT_tstStdCANMessage *stSTDCan );
extern void PORT_g_fvCANTPStop			    ( void );

extern PORT_tenAPPReturns PORT_g_fvCANTPReadMessage ( PORT_tstStdCANMessage *stSTDCan );
extern bool PORT_g_fvCANTPMessageAvailable          ( void );
extern PORT_tenAPPReturns PORT_g_fvCANTPMessageAck  ( PORT_tenMessageAckState enMessageState );
extern PORT_tenAPPReturns PORT_l_fenPrintCAN        ( PORT_tstStdCANMessage * stSTDCan );
extern PORT_tenAPPReturns PORT_l_fenPrintCANMultiframeType ( PORT_tstStdCANMessage * stSTDCan );

extern void PORT_g_fvCANMultiFrameClientInit        ( void );
extern void PORT_g_fvCANMultiFrameClientHandler     ( void );
extern void PORT_g_fvCANMultiFrameClientStop	    ( void );

extern void PORT_g_fvCANMultiFrameServerInit        ( void );
extern void PORT_g_fvCANMultiFrameServerHandler     ( void );
extern void PORT_g_fvCANMultiFrameServerStop        ( void );

#endif // _CAN_CAN_TP_E_H_
/******************************************************************************
 * EOF: CAN_TP_e.h
 *******************************************************************************/
