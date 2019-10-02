/*
 * DDM 01/10/2019
 *  */

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "CAN_TP_e.h"

PORT_tstStdCANMessage PORT_l_stStandardCANMessageTP;

PORT_tenMessageAckState PORT_l_enMessageAckState;

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the CAN TP.
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
void PORT_g_fvCANTPInit(void)
{
  PORT_l_enMessageAckState = hPORT_nenMessageAck;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine is the handler for the CAN TP.
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
void PORT_g_fvCANTPHandler(void)
{
  /* will not be used as cyclic handler. Other components will hadnle the mechanism. */
}

PORT_tenAPPReturns PORT_g_fvCANTPSendMessage(PORT_tstStdCANMessage *stSTDCan)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  if(NULL != stSTDCan)
  {
    if( hPORT_nenMessageAck == PORT_l_enMessageAckState)
    {
      /* If the TP message is free. */
      PORT_l_stStandardCANMessageTP = *stSTDCan;
      PORT_l_enMessageAckState = hPORT_nenMessageNotAck;

      enRetCode = hPORT_nenSuccess;
    }
    else
    {
      /* The TP message is Busy. */
      enRetCode = hPORT_nenBusy;
    }
  }
  else
  {
    enRetCode = hPORT_nenNULLPointer;
  }

  return enRetCode;
}

PORT_tenAPPReturns PORT_g_fvCANTPReadMessage(PORT_tstStdCANMessage *stSTDCan)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  if(NULL != stSTDCan)
  {
    if( hPORT_nenMessageAck == PORT_l_enMessageAckState)
    {
      /* If the TP message is free. No message. */
      enRetCode = hPORT_nenFailure;
    }
    else
    {
      /* The TP message is available. */
      *stSTDCan = PORT_l_stStandardCANMessageTP;
      enRetCode = hPORT_nenSuccess;
    }
  }
  else
  {
    enRetCode = hPORT_nenNULLPointer;
  }

  return enRetCode;
}

bool PORT_g_fvCANTPMessageAvailable(void)
{
  bool bRet = false;

  if( hPORT_nenMessageNotAck == PORT_l_enMessageAckState)
  {
    bRet = true;
  }
  else
  {
    /* Already the return code with value false. */
  }

  return bRet;
}

PORT_tenAPPReturns PORT_g_fvCANTPMessageAck( PORT_tenMessageAckState enMessageState )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  if( hPORT_nenMessageAck == enMessageState)
  {
    PORT_l_enMessageAckState = hPORT_nenMessageAck;
    enRetCode = hPORT_nenSuccess;
  }
  else
  {
    enRetCode = hPORT_nenSuccess;
  }

  return enRetCode;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the stop for the CAN TP.
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
void PORT_g_fvCANTPStop(void)
{

}

PORT_tenAPPReturns PORT_l_fenPrintCAN( PORT_tstStdCANMessage * stSTDCan )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8Index;

  if(NULL != stSTDCan)
  {
    DDM_DBG("\r\n <_<_<_<_<_<_<_<_<- CAN Message printing start ->_>_>_>_>_>_>_>_>_>");

    DDM_DBG("\r\n CAN Identifier: 0x");
#if PORT_PRINTS_ENABLED == SET_VALUE_1B
  printf("%04X", stSTDCan->unIDF.u16IDF);
#endif

    DDM_DBG("\r\n CAN DLC: ");
#if PORT_PRINTS_ENABLED == SET_VALUE_1B
  printf("%d", stSTDCan->unDLC.u8DLC);
#endif
    DDM_DBG("\r\n CAN Payload hex: ");

    for(u8Index = CLEAR_VALUE_1B; u8Index < stSTDCan->unDLC.u8DLC; u8Index++)
    {
#if PORT_PRINTS_ENABLED == SET_VALUE_1B
    printf(" %02X", stSTDCan->au8DataPayload[u8Index]);
    fflush(stdout);
#endif
    }

    DDM_DBG("\r\n <_<_<_<_<_<_<_<_<- CAN Message printing end ->_>_>_>_>_>_>_>_>_>");

    enRetCode = hPORT_nenSuccess;
  }
  else
  {
    enRetCode = hPORT_nenNULLPointer;
  }

  return enRetCode;
}

PORT_tenAPPReturns PORT_l_fenPrintCANMultiframeType( PORT_tstStdCANMessage * stSTDCan )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8CanType;
  if(NULL != stSTDCan)
  {
    u8CanType = ((uint8)(((stSTDCan->au8DataPayload[CLEAR_VALUE_1B]) & MASK_FRAME_TYPE) >> NIBLE_SHIFT));

    switch(u8CanType)
    {
      case SINGLE_FRAME:
      {
        DDM_DBG("\r\n Single Frame CAN Message");
        break;
      }

      case FIRST_FRAME:
      {
        DDM_DBG("\r\n First Frame CAN Message");
        break;
      }

      case CONSECUTIVE_FRAME:
      {
        DDM_DBG("\r\n Consecutive Frame CAN Message");
        break;
      }

      case FLOW_FRAME:
      {
        DDM_DBG("\r\n Flow Frame CAN Message");
        break;
      }

      default:
      {
        DDM_DBG("\r\n Default CAN Message");
        break;
      }
    }

    enRetCode = hPORT_nenSuccess;
  }
  else
  {
    enRetCode = hPORT_nenNULLPointer;
  }

  return enRetCode;
}

/******************************************************************************
 * EOF: CAN_MultiFrameTP.c
 *******************************************************************************/
