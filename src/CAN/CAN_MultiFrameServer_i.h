/*
 * DDM 02/10/2019
 *  */

#ifndef _CAN_CAN_MULTIFRAMESERVER_I_H_
#define _CAN_CAN_MULTIFRAMESERVER_I_H_
//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "../ForOthers.h"

#define BLOCK_SIZE_MTF 0x03u
#define SEP_TIME_MIN   0x0Au


typedef enum
{

  hPORT_nenMultiframeServerReceiveFirstFrame           = 0x00u,    /* Success operation return */
  hPORT_nenMultiframeServerSendFlowControl,
  hPORT_nenMultiframeServerReceiveBlockFrame,
  hPORT_nenMultiframeClientReceiveSingleFrame,

  hPORT_nenMultiframeServerTotalSteps
}PORT_tenMultiframeServerSteps;

#endif // _CAN_CAN_MULTIFRAMESERVER_I_H_
/******************************************************************************
 * EOF: CAN_MultiFrameServer_i.h
 *******************************************************************************/
