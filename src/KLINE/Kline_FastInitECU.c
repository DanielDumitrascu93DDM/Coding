/*
 * DDM 01/10/2019
 *  */

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "Kline_FastInitECU_e.h"
#include "Kline_FastInitECU_i.h"

#include "Kline_FastInitTester_e.h"

void PORT_g_fvKlineECUIdleMechanism      ( void );
PORT_tenAPPReturns PORT_g_fvKlineECUTX   ( uint8 u8DataByte );

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the Kline ECU.
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
void PORT_g_fvKlineECUInit(void)
{
  PORT_l_stKlineECU.au8RXBuffer = &PORT_l_au8KlineECURXBuffer[BUFFER_FIRST_ELEMENT];
  PORT_l_stKlineECU.u8IndexRX = BUFFER_FIRST_ELEMENT;
  PORT_l_stKlineECU.u8IndexTX = BUFFER_FIRST_ELEMENT;

  PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineECUIdle;
  PORT_l_stKlineECU.enECUOldStateMachine = hPORT_nenKlineECUIdle;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the Kline ECU.
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
void PORT_g_fvKlineECUHandler(void)
{
  switch(PORT_l_stKlineECU.enECUActualStateMachine)
  {
    PORT_tenAPPReturns enRetOperation = hPORT_nenUndefinedReturn;
    PORT_tenAPPReturns enRetOperation2 = hPORT_nenUndefinedReturn;

    case hPORT_nenKlineECUIdle:
    {
      PORT_g_fvKlineECUIdleMechanism();
      break;
    }

    case hPORT_nenKlineECURecvAddress:
    {
      if(BUFFER_ONE_ELEMENT == PORT_l_stKlineECU.u8IndexRX)
      {
        /* We expect to be the address byte. */
        if(KLINE_ADDRESS == PORT_l_stKlineECU.au8RXBuffer[BUFFER_FIRST_ELEMENT])
        {
          DDM_DBG("\r\n Kline ECU Valid Address. Transition to Send SYNC Byte");

          PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
          PORT_l_stKlineECU.enECUActualStateMachine =hPORT_nenKlineECUSendSyncByte;
          PORT_l_stKlineECU.u8IndexRX--;
        }
        else
        {

        }
      }
      else
      {

      }

      break;
    }

    case hPORT_nenKlineECUSendSyncByte:
    {
      enRetOperation = PORT_g_fvKlineECUTX(KLINE_SYNC);

      DDM_DBG("\r\n Kline ECU Send SYNC.");

      if(hPORT_nenSuccess == enRetOperation)
      {
        PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
        PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineTSTIdle;
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }
      PORT_g_fvDecideTheReturn(enRetOperation);

      break;
    }

    case hPORT_nenKlineECUSendKeyBytes:
    {
      enRetOperation = PORT_g_fvKlineECUTX(KLINE_KEY_BYTE1);

      DDM_DBG("\r\n Kline ECU Send KeyByte1.");

      if(hPORT_nenSuccess == enRetOperation)
      {
        /* We will make the decision lower. */
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }
      PORT_g_fvDecideTheReturn(enRetOperation);

      enRetOperation2 = PORT_g_fvKlineECUTX(KLINE_KEY_BYTE2);

      DDM_DBG("\r\n Kline ECU Send KeyByte2.");

      if(hPORT_nenSuccess == enRetOperation2)
      {
        /* We will make the decision lower. */
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }

      PORT_g_fvDecideTheReturn(enRetOperation2);

      if( (hPORT_nenSuccess == enRetOperation) && (hPORT_nenSuccess == enRetOperation2))
      {
        DDM_DBG("\r\n Kline ECU Sent KeyBytes. Transition To Idle");

        PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
        PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineTSTIdle;
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }

      break;
    }

    case hPORT_nenKlineECURecvKeyByte2Inv:
    {
      if(BUFFER_ONE_ELEMENT == PORT_l_stKlineECU.u8IndexRX)
      {
        /* We expect to be the address byte. */
        if(((uint8)(~KLINE_KEY_BYTE2)) == PORT_l_stKlineECU.au8RXBuffer[BUFFER_FIRST_ELEMENT])
        {
          DDM_DBG("\r\n Kline ECU Valid Inverted KeyByte2. Transition to Send Inverted Address");
          PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
          PORT_l_stKlineECU.enECUActualStateMachine =hPORT_nenKlineECUSendAddressInv;
          PORT_l_stKlineECU.u8IndexRX--;
        }
        else
        {

        }
      }
      else
      {

      }

      break;
    }

    case hPORT_nenKlineECUSendAddressInv:
    {
      enRetOperation = PORT_g_fvKlineECUTX(~KLINE_ADDRESS);

      DDM_DBG("\r\n Kline ECU Send Inverted Address. GO in Idle");

      if(hPORT_nenSuccess == enRetOperation)
      {
        PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
        PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineTSTIdle;
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }
      PORT_g_fvDecideTheReturn(enRetOperation);

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
 *                       This routine handles the initialization for the Kline ECU.
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
void PORT_g_fvKlineECUIdleMechanism(void)
{
  switch(PORT_l_stKlineECU.enECUOldStateMachine)
  {
    case hPORT_nenKlineECUIdle:
    {
      if( BUFFER_FIRST_ELEMENT != PORT_l_stKlineECU.u8IndexRX )
      {
        /* We expected the address byte. Let's check. */
        PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
        PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineECURecvAddress;

        DDM_DBG("\r\n Kline ECU Transition From IDLE to RECV Address.");
      }
      else
      {
        /* Do nothing! */
      }

      break;
    }

    case hPORT_nenKlineECURecvAddress:
    {

      break;
    }

    case hPORT_nenKlineECUSendSyncByte:
    {
      PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
      PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineECUSendKeyBytes;

      DDM_DBG("\r\n Kline ECU Transition From IDLE to Send Key Bytes.");

      break;
    }

    case hPORT_nenKlineECUSendKeyBytes:
    {
      if( BUFFER_FIRST_ELEMENT != PORT_l_stKlineECU.u8IndexRX )
      {
        /* We expected the address byte. Let's check. */
        PORT_l_stKlineECU.enECUOldStateMachine = PORT_l_stKlineECU.enECUActualStateMachine;
        PORT_l_stKlineECU.enECUActualStateMachine = hPORT_nenKlineECURecvKeyByte2Inv;

        DDM_DBG("\r\n Kline ECU Transition From IDLE to RECV KeyByte2 Inverted.");
      }
      else
      {
        /* Do nothing! */
      }
      break;
    }

    case hPORT_nenKlineECURecvKeyByte2Inv:
    {

      break;
    }

    case hPORT_nenKlineECUSendAddressInv:
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
 *                       This routine handles the Transmission for the Kline ECU.
 *
 * @par Calibration:
 *                       none
 *
 * @param
 *                       uint8 data byte
 *
 * @return
 *                       PORT_tenAPPReturns
 * @req
 *                       Satisfies the following requirements or architecture tags
 *
 * @test
 *                       Module Test Case IDs for traceability
 *******************************************************************************/
PORT_tenAPPReturns PORT_g_fvKlineECUTX(uint8 u8DataByte)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  enRetCode = PORT_g_fvKlineTesterRX(u8DataByte);

  return enRetCode;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the Reception for the Kline ECU.
 *
 * @par Calibration:
 *                       none
 *
 * @param
 *                       uint8 data byte
 *
 * @return
 *                       PORT_tenAPPReturns
 * @req
 *                       Satisfies the following requirements or architecture tags
 *
 * @test
 *                       Module Test Case IDs for traceability
 *******************************************************************************/
PORT_tenAPPReturns PORT_g_fvKlineECURX(uint8 u8DataByte)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  if( KLINE_BUFFER_MAX > PORT_l_stKlineECU.u8IndexRX )
  {
    PORT_l_stKlineECU.au8RXBuffer[PORT_l_stKlineECU.u8IndexRX] = u8DataByte;
    PORT_l_stKlineECU.u8IndexRX++;
    enRetCode = hPORT_nenSuccess;
  }
  else
  {
    /* We are full! */
    enRetCode = hPORT_nenFullReturn;
  }

  return enRetCode;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the stop for the Kline ECU.
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
void PORT_g_fvKlineECUStop(void)
{

}
/******************************************************************************
 * EOF: Kline_FastInitECU.c
 *******************************************************************************/
