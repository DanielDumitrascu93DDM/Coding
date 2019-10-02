/*
 * DDM 01/10/2019
 *  */

#ifndef _CAN_CAN_TP_T_H_
#define _CAN_CAN_TP_T_H_

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "CAN_Stuffing_t.h"

#define MASK_FRAME_TYPE     0xF0u
#define SINGLE_FRAME        0x00u
#define FIRST_FRAME         0x01u
#define CONSECUTIVE_FRAME   0x02u
#define FLOW_FRAME          0x03u
#define FLOW_STATUS_0       0x00u
#define FLOW_STATUS_1       0x01u
#define FLOW_STATUS_2       0x02u

#define NIBLE_SHIFT         0x04u

#define MULTIFRAME_CLIENT_IDF 0x55AAu
#define MULTIFRAME_SERVER_IDF 0xAA55u

typedef enum
{

  hPORT_nenMessageNotAck           = 0x00u,    /* Success operation return */
  hPORT_nenMessageAck              ,

  hPORT_nenTotalMessageStates

}PORT_tenMessageAckState;

typedef struct
{

  uint8 u8BlockSize;
  uint8 u8SeparationTime;

}PORT_tstMultiFrameReqs;

#endif // _CAN_CAN_TP_T_H_
/******************************************************************************
 * EOF: CAN_TP_t.h
 *******************************************************************************/
