/*
 * DDM 27/09/2019
 *  */

#ifndef _OS_H_
#define _OS_H_

//===============================================================================
//      Own Header file
//===============================================================================
#include "defines.h"
#include "CAN/CAN_Stuffing_e.h"
#include "CAN/CAN_TP_e.h"

#include "KLINE/Kline_FastInitECU_e.h"
#include "KLINE/Kline_FastInitTester_e.h"

#include "LIN/LIN_e.h"

typedef void (*PORT_pfStartFunctionType)   (void);
typedef void (*PORT_pfStopFunctionType)   (void);
typedef void (*PORT_pfHandlerFunctionType)   (void);

typedef enum
{

  hPORT_nenAPPEnableHandlers         = 0x00u,
  hPORT_nenAPPInit                   ,
  hPORT_nenAPPHandlig                ,
  hPORT_nenAPPEnd                ,

  hPORT_nenTotalSMStates
}PORT_tenApplicationSM;

typedef enum
{

  hPORT_nenHandlerDisabled        = 0x00u,
  hPORT_nenHandlerEnabled         ,

  hPORT_nenTotalHandlerStates

}PORT_tenHandlerState;

typedef struct
{

  PORT_pfStartFunctionType       pfStartFunction;
  PORT_pfStopFunctionType        pfStopFunction;
  PORT_pfHandlerFunctionType     pfHandler;
  PORT_tenHandlerState           state;

}PORT_tstModuleCallBacksHook;

typedef enum
{

  hPORT_nenHandlerCANStuffing        = 0x00u,
  hPORT_nenHandlerCANDestuffing      ,
  hPORT_nenHandlerKlineFastInitTester,
  hPORT_nenHandlerKlineFastInitECU   ,
  hPORT_nenHandlerCANTP              ,
  hPORT_nenHandlerCANMultiFrameClient,
  hPORT_nenHandlerCANMultiFrameServer,
  hPORT_nenHandlerLINFrame,

  hPMG_nenTotalHandler

}PORT_tenHandlers;

static PORT_tstModuleCallBacksHook PORT_stModuleCallBacksHook [] =
{
 /**start function*/                            /**stop function*/                      /**handler function*/                     /** State of handler: enabled/disabled*/
 {&PORT_g_fvCANStuffingInit,                    &PORT_g_fvCANStuffingSTOP,           &PORT_g_fvCANStuffingHandler,                hPORT_nenHandlerDisabled },
 {&PORT_g_fvCANDestuffingInit,                  &PORT_g_fvCANDestuffingStop,         &PORT_g_fvCANDestuffingHandler,              hPORT_nenHandlerDisabled },
 {&PORT_g_fvKlineTesterInit,                    &PORT_g_fvKlineTesterStop,           &PORT_g_fvKlineTesterHandler,                hPORT_nenHandlerDisabled },
 {&PORT_g_fvKlineECUInit,                       &PORT_g_fvKlineECUStop,              &PORT_g_fvKlineECUHandler,                   hPORT_nenHandlerDisabled },
 {&PORT_g_fvCANTPInit,                          &PORT_g_fvCANTPStop,                 &PORT_g_fvCANTPHandler,                      hPORT_nenHandlerDisabled },
 {&PORT_g_fvCANMultiFrameClientInit,            &PORT_g_fvCANMultiFrameClientStop,   &PORT_g_fvCANMultiFrameClientHandler,        hPORT_nenHandlerDisabled },
 {&PORT_g_fvCANMultiFrameServerInit,            &PORT_g_fvCANMultiFrameServerStop,   &PORT_g_fvCANMultiFrameServerHandler,        hPORT_nenHandlerDisabled },
 {&PORT_g_fvLINInit,            				&PORT_g_fvLINStop,    			 	 &PORT_g_fvLINHandler,        				  hPORT_nenHandlerDisabled },

};

PORT_tenApplicationSM PORT_l_enApplicationStateMachine = hPORT_nenAPPEnableHandlers;

#endif // _OS_H_
/******************************************************************************
 * EOF: os.h
 *******************************************************************************/
