/*
 * DDM 01/10/2019
 *  */

#ifndef _CAN_CAN_MULTIFRAMECLIENT_I_H_
#define _CAN_CAN_MULTIFRAMECLIENT_I_H_
//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "../ForOthers.h"

#define MULTIFRAME_DLC 0x14u

typedef enum
{

  hPORT_nenMultiframeClientSendFirstFrame           = 0x00u,    /* Success operation return */
  hPORT_nenMultiframeClientWaitFlowControl,
  hPORT_nenMultiframeClientSendBlockFrame,
  hPORT_nenMultiframeClientSendSingleFrame,

  hPORT_nenMultiframeClientTotalSteps
}PORT_tenMultiframeClientSteps;

#endif // _CAN_CAN_MULTIFRAMECLIENT_I_H_
/******************************************************************************
 * EOF: CAN_MultiFrameClient_i.h
 *******************************************************************************/
