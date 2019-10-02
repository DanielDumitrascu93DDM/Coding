/*
 * DDM 01/10/2019
 *  */

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "Kline_FastInitTester_e.h"
#include "Kline_FastInitTester_i.h"

#include "Kline_FastInitECU_e.h"

void PORT_g_fvKlineTesterIdleMechanism      ( void );
PORT_tenAPPReturns PORT_g_fvKlineTesterTX   ( uint8 u8DataByte );

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the Kline Tester.
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
void PORT_g_fvKlineTesterInit(void)
{
  PORT_l_stKlineTester.au8RXBuffer = &PORT_l_au8KlineTesterRXBuffer[BUFFER_FIRST_ELEMENT];
  PORT_l_stKlineTester.u8IndexRX = BUFFER_FIRST_ELEMENT;
  PORT_l_stKlineTester.u8IndexTX = BUFFER_FIRST_ELEMENT;

  PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTSendAddress;
  PORT_l_stKlineTester.enTSTOldStateMachine = hPORT_nenKlineTSTIdle;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the Kline Tester.
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
void PORT_g_fvKlineTesterHandler(void)
{
  PORT_tenAPPReturns enRetOperation = hPORT_nenUndefinedReturn;

  switch(PORT_l_stKlineTester.enTSTActualStateMachine)
  {
    case hPORT_nenKlineTSTIdle:
    {
      PORT_g_fvKlineTesterIdleMechanism();

      break;
    }

    case hPORT_nenKlineTSTSendAddress:
    {
      enRetOperation = PORT_g_fvKlineTesterTX(KLINE_ADDRESS);

      DDM_DBG("\r\n Kline Tester Send Address.");

      if(hPORT_nenSuccess == enRetOperation)
      {
        PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
        PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTIdle;
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }
      PORT_g_fvDecideTheReturn(enRetOperation);

      break;
    }

    case hPORT_nenKlineTSTRecvSyncByte:
    {
      if(BUFFER_ONE_ELEMENT == PORT_l_stKlineTester.u8IndexRX)
      {
        /* We expect to be the address byte. */
        if(KLINE_SYNC == PORT_l_stKlineTester.au8RXBuffer[BUFFER_FIRST_ELEMENT])
        {
          DDM_DBG("\r\n Kline Tester Synchronized. Go in Idle");

          PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
          PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTIdle;
          PORT_l_stKlineTester.u8IndexRX--;
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

    case hPORT_nenKlineTSTRecvKeyBytes:
    {
      if(BUFFER_TWO_ELEMENTs == PORT_l_stKlineTester.u8IndexRX)
      {
        /* We expect to be the address byte. */
        if( (KLINE_KEY_BYTE1 == PORT_l_stKlineTester.au8RXBuffer[BUFFER_FIRST_ELEMENT])
            && (KLINE_KEY_BYTE2 == PORT_l_stKlineTester.au8RXBuffer[BUFFER_SECOND_ELEMENT]) )
        {
          DDM_DBG("\r\n Kline Tester Valid KeyBytes Received. Go in Send KeyByte2 Inverted");

          PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
          PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTIdle;
          PORT_l_stKlineTester.u8IndexRX--;
          PORT_l_stKlineTester.u8IndexRX--;
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

    case hPORT_nenKlineTSTSendKeyByte2Inv:
    {
      enRetOperation = PORT_g_fvKlineTesterTX(~KLINE_KEY_BYTE2);

      DDM_DBG("\r\n Kline Tester Send KeyByte2 Inverted.");

      if(hPORT_nenSuccess == enRetOperation)
      {
        PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
        PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTIdle;
      }
      else
      {
        /* Maybe buffer full on the RX side or a failure. We will retry.
         * If it's a failure we will be stuck here. */
      }
      PORT_g_fvDecideTheReturn(enRetOperation);

      break;
    }

    case hPORT_nenKlineTSTRecvAddressInv:
    {
      if(BUFFER_ONE_ELEMENT == PORT_l_stKlineTester.u8IndexRX)
      {
        /* We expect to be the address byte. */
        if(((uint8)(~KLINE_ADDRESS)) == PORT_l_stKlineTester.au8RXBuffer[BUFFER_FIRST_ELEMENT])
        {
          DDM_DBG("\r\n Kline Tester Received Valid inverted Address. Go in Idle");
          PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
          PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTIdle;
          PORT_l_stKlineTester.u8IndexRX--;
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

    default: break;
  }
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the Kline Tester.
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
void PORT_g_fvKlineTesterIdleMechanism(void)
{
  switch(PORT_l_stKlineTester.enTSTOldStateMachine)
  {
    case hPORT_nenKlineTSTIdle:
    {

      break;
    }

    case hPORT_nenKlineTSTSendAddress:
    {
      if( BUFFER_FIRST_ELEMENT != PORT_l_stKlineTester.u8IndexRX )
      {
        /* We expected the address byte. Let's check. */
        PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
        PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTRecvSyncByte;

        DDM_DBG("\r\n Kline Tester Transition From IDLE to RECV SYNC byte.");
      }
      else
      {
        /* Do nothing! */
      }

      break;
    }

    case hPORT_nenKlineTSTRecvSyncByte:
    {
      if( BUFFER_TWO_ELEMENTs == PORT_l_stKlineTester.u8IndexRX )
      {
        /* We expected the address byte. Let's check. */
        PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
        PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTRecvKeyBytes;

        DDM_DBG("\r\n Kline Tester Transition From IDLE to RECV Key bytes.");
      }
      else
      {
        /* Do nothing! */
      }

      break;
    }

    case hPORT_nenKlineTSTRecvKeyBytes:
    {
      PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
      PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTSendKeyByte2Inv;

      break;
    }

    case hPORT_nenKlineTSTSendKeyByte2Inv:
    {
      if( BUFFER_FIRST_ELEMENT != PORT_l_stKlineTester.u8IndexRX )
      {
        /* We expected the address byte. Let's check. */
        PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
        PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTRecvAddressInv;

        DDM_DBG("\r\n Kline Tester Transition From IDLE to RECV Inverted Address.");
      }
      else
      {
        /* Do nothing! */
      }

      break;
    }

    case hPORT_nenKlineTSTRecvAddressInv:
    {
      PORT_l_stKlineTester.enTSTOldStateMachine = PORT_l_stKlineTester.enTSTActualStateMachine;
      PORT_l_stKlineTester.enTSTActualStateMachine = hPORT_nenKlineTSTSMTotalSteps;

      DDM_DBG("\r\n Kline Fast Initialization Successfully Ended!");

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
 *                       This routine handles the Transmission for the Kline Tester.
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
PORT_tenAPPReturns PORT_g_fvKlineTesterTX(uint8 u8DataByte)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  enRetCode = PORT_g_fvKlineECURX(u8DataByte);

  return enRetCode;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the Reception for the Kline Tester.
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
PORT_tenAPPReturns PORT_g_fvKlineTesterRX(uint8 u8DataByte)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  if( KLINE_BUFFER_MAX > PORT_l_stKlineTester.u8IndexRX )
  {
    PORT_l_stKlineTester.au8RXBuffer[PORT_l_stKlineTester.u8IndexRX] = u8DataByte;
    PORT_l_stKlineTester.u8IndexRX++;
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
 *                       This routine handles the stop for the Kline Tester.
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
void PORT_g_fvKlineTesterStop(void)
{

}
/******************************************************************************
 * EOF: Kline_FastInitTester.c
 *******************************************************************************/
