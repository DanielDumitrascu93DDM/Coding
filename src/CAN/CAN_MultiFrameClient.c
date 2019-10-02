/*
 * DDM 01/10/2019
 *  */

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "CAN_Stuffing_t.h"
#include "CAN_TP_e.h"
#include "CAN_MultiFrameClient_i.h"

static PORT_tenMultiframeClientSteps PORT_l_enMultiframeClientSteps;
static PORT_tstStdCANMessage PORT_l_stCANMultiFrame;
static uint8 PORT_l_u8MultiframePayload[CAN_STD_PAYLOAD];
static PORT_tstMultiFrameReqs PORT_l_stMultiFrameReqs;


PORT_tenAPPReturns PORT_l_fenUnpackMultiframeRules( PORT_tstStdCANMessage * stSTDCan );

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the CAN
 *                       Multiframe Client.
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
void PORT_g_fvCANMultiFrameClientInit(void)
{
  PORT_l_enMultiframeClientSteps = hPORT_nenMultiframeClientSendFirstFrame;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine is the handler for the CAN Multiframe Client.
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
void PORT_g_fvCANMultiFrameClientHandler(void)
{
  PORT_tenAPPReturns enCallReturn = hPORT_nenUndefinedReturn;

  switch(PORT_l_enMultiframeClientSteps)
  {
    case hPORT_nenMultiframeClientSendFirstFrame:
    {
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE0] = ((uint8) (FIRST_FRAME << NIBLE_SHIFT));
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE1] = MULTIFRAME_DLC;
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE2] = CAN_DEFAULT_REG_VAL;
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE3] = 0x00;
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE4] = 0x01;
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE5] = 0x02;
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE6] = 0x03;
      PORT_l_u8MultiframePayload[CAN_PAYLOAD_BYTE7] = 0x04;

      PORT_l_stCANMultiFrame.unSOF.u8SOF = CAN_SOF_VALUE;
      PORT_l_stCANMultiFrame.unIDF.u16IDF = (uint16)MULTIFRAME_SERVER_IDF;
      PORT_l_stCANMultiFrame.unRTR.u8RTR = CAN_DEFAULT_REG_VAL;
      PORT_l_stCANMultiFrame.unIDE.u8IDE = CAN_DEFAULT_REG_VAL;
      PORT_l_stCANMultiFrame.unRB0.u8RB0 = CAN_DEFAULT_REG_VAL;
      PORT_l_stCANMultiFrame.unDLC.u8DLC = 0x08u;
      PORT_l_stCANMultiFrame.au8DataPayload = &PORT_l_u8MultiframePayload[0];

      if( hPORT_nenSuccess == PORT_g_fvCANTPSendMessage(&PORT_l_stCANMultiFrame))
      {
        DDM_DBG("\r\n Successfully sent the First Frame!");
        PORT_l_enMultiframeClientSteps = hPORT_nenMultiframeClientWaitFlowControl;
      }
      else
      {
        /* Maybe it is busy. */
        DDM_DBG("\r\n Failed to send the First Frame!");
      }

      break;
    }

    case hPORT_nenMultiframeClientWaitFlowControl:
    {
      if(true == PORT_g_fvCANTPMessageAvailable())
      {
        DDM_DBG("\r\n Multiframe Client Read a message available");
        enCallReturn = PORT_g_fvCANTPReadMessage(&PORT_l_stCANMultiFrame);

        PORT_g_fvDecideTheReturn(enCallReturn);

        if( hPORT_nenSuccess == enCallReturn )
        {
          /* Successfully read a message. */
          if( ((uint16)MULTIFRAME_CLIENT_IDF) == PORT_l_stCANMultiFrame.unIDF.u16IDF)
          {
            /* Valid destination. */
            DDM_DBG("\r\n Multiframe Client Read a targeted message.");

            PORT_l_fenPrintCANMultiframeType(&PORT_l_stCANMultiFrame);
            PORT_l_fenPrintCAN(&PORT_l_stCANMultiFrame);
            PORT_g_fvCANTPMessageAck(hPORT_nenMessageAck);

            if( hPORT_nenSuccess == PORT_l_fenUnpackMultiframeRules(&PORT_l_stCANMultiFrame) )
            {
              PORT_l_enMultiframeClientSteps = hPORT_nenMultiframeClientSendBlockFrame;
            }
            else
            {

            }
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

    case hPORT_nenMultiframeClientSendBlockFrame:
    {
      break;
    }

    default: break;
  }
}

PORT_tenAPPReturns PORT_l_fenUnpackMultiframeRules( PORT_tstStdCANMessage * stSTDCan )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  if(NULL != stSTDCan)
  {
    DDM_DBG("\r\n Client Flow Control Frame updating reqs...");
    PORT_l_stMultiFrameReqs.u8BlockSize = stSTDCan->au8DataPayload[FLOW_BS_OFFSET];
    PORT_l_stMultiFrameReqs.u8SeparationTime = stSTDCan->au8DataPayload[FLOW_STMIN_OFFSET];
    DDM_DBG("\r\n Client Flow Control Frame updated reqs.");

    enRetCode = hPORT_nenSuccess;
  }
  else
  {
    enRetCode = hPORT_nenNULLPointer;
  }


  return enRetCode;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the stop for the CAN
 *                       Multiframe Client.
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
void PORT_g_fvCANMultiFrameClientStop(void)
{

}

/******************************************************************************
 * EOF: CAN_MultiFrame.c
 *******************************************************************************/
