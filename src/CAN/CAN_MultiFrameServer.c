/*
 * DDM 01/10/2019
 *  */

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "CAN_Stuffing_t.h"
#include "CAN_TP_e.h"
#include "CAN_MultiFrameServer_i.h"


static PORT_tenMultiframeServerSteps PORT_l_enMultiframeServerSteps;
static PORT_tstStdCANMessage PORT_l_stCANMultiFrameServer;
static uint8 PORT_l_u8MultiframePayloadServer[CAN_STD_PAYLOAD];

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the CAN
 *                       Multiframe Server.
 *
 * @par Calibration:
 *                       none
 *
 * @param
 *                       none
 *
 * @return
 *                       none
 * @req
 *                       Satisfies the following requirements or architecture tags
 *
 * @test
 *                       Module Test Case IDs for traceability
 *******************************************************************************/
void PORT_g_fvCANMultiFrameServerInit(void)
{
  PORT_l_enMultiframeServerSteps = hPORT_nenMultiframeServerReceiveFirstFrame;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine is the handler for the CAN Multiframe Server.
 *
 * @par Calibration:
 *                       none
 *
 * @param
 *                       none
 *
 * @return
 *                       none
 * @req
 *                       Satisfies the following requirements or architecture tags
 *
 * @test
 *                       Module Test Case IDs for traceability
 *******************************************************************************/
void PORT_g_fvCANMultiFrameServerHandler(void)
{
  PORT_tenAPPReturns enCallReturn = hPORT_nenUndefinedReturn;

  switch(PORT_l_enMultiframeServerSteps)
  {
    case hPORT_nenMultiframeServerReceiveFirstFrame:
    {
      if(true == PORT_g_fvCANTPMessageAvailable())
      {
        DDM_DBG("\r\n Multiframe Server Read a message available");
        enCallReturn = PORT_g_fvCANTPReadMessage(&PORT_l_stCANMultiFrameServer);

        PORT_g_fvDecideTheReturn(enCallReturn);

        if( hPORT_nenSuccess == enCallReturn )
        {
          /* Successfully read a message. */
          if( ((uint16)MULTIFRAME_SERVER_IDF) == PORT_l_stCANMultiFrameServer.unIDF.u16IDF)
          {
            /* Valid destination. */
            DDM_DBG("\r\n Multiframe Server Read a targeted message.");

            PORT_l_fenPrintCANMultiframeType(&PORT_l_stCANMultiFrameServer);
            PORT_l_fenPrintCAN(&PORT_l_stCANMultiFrameServer);
            PORT_g_fvCANTPMessageAck(hPORT_nenMessageAck);
            PORT_l_enMultiframeServerSteps = hPORT_nenMultiframeServerSendFlowControl;
          }
          else
          {
            /* Invalid destination. */
            PORT_g_fvCANTPMessageAck(hPORT_nenMessageNotAck);
          }
        }
        else
        {
          /* Unsuccessfully read a message. */
        }
      }
      else
      {
        /* No message available. */
      }

      break;
    }

    case hPORT_nenMultiframeServerSendFlowControl:
    {
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE0] = ((uint8) (FLOW_FRAME << NIBLE_SHIFT) | (FLOW_STATUS_0));
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE1] = BLOCK_SIZE_MTF;
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE2] = SEP_TIME_MIN;
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE3] = CAN_PADDING;
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE4] = CAN_PADDING;
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE5] = CAN_PADDING;
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE6] = CAN_PADDING;
      PORT_l_u8MultiframePayloadServer[CAN_PAYLOAD_BYTE7] = CAN_PADDING;

      PORT_l_stCANMultiFrameServer.unSOF.u8SOF = CAN_SOF_VALUE;
      PORT_l_stCANMultiFrameServer.unIDF.u16IDF = (uint16)MULTIFRAME_CLIENT_IDF;
      PORT_l_stCANMultiFrameServer.unRTR.u8RTR = CAN_DEFAULT_REG_VAL;
      PORT_l_stCANMultiFrameServer.unIDE.u8IDE = CAN_DEFAULT_REG_VAL;
      PORT_l_stCANMultiFrameServer.unRB0.u8RB0 = CAN_DEFAULT_REG_VAL;
      PORT_l_stCANMultiFrameServer.unDLC.u8DLC = 0x08u;
      PORT_l_stCANMultiFrameServer.au8DataPayload = &PORT_l_u8MultiframePayloadServer[0];

      if( hPORT_nenSuccess == PORT_g_fvCANTPSendMessage(&PORT_l_stCANMultiFrameServer))
      {
        DDM_DBG("\r\n Server Successfully sent the Flow Control Frame!");
        PORT_l_enMultiframeServerSteps = hPORT_nenMultiframeServerReceiveBlockFrame;
      }
      else
      {
        /* Maybe it is busy. */
        DDM_DBG("\r\n Server Failed to send the Flow Control Frame!");
      }

      break;
    }

    case hPORT_nenMultiframeServerReceiveBlockFrame:
    {

      break;
    }

    default: break;

  }
}


/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the stop for the CAN
 *                       Multiframe Server.
 *
 * @par Calibration:
 *                       none
 *
 * @param
 *                       none
 *
 * @return
 *                       none
 * @req
 *                       Satisfies the following requirements or architecture tags
 *
 * @test
 *                       Module Test Case IDs for traceability
 *******************************************************************************/
void PORT_g_fvCANMultiFrameServerStop(void)
{

}
/******************************************************************************
 * EOF: CAN_MultiFrameServer.c
 *******************************************************************************/
