/*
 * DDM 27/09/2019
 *  */

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "CAN_Stuffing_i.h"
#include "CAN_Stuffing_e.h"

PORT_tenAPPReturns PORT_l_fenParseSTDMessage					( PORT_tstStdCANMessage * stSTDMessage );
PORT_tenAPPReturns PORT_l_fenPrintBitsOfMessage					( uint8 * pu8Var, uint8 u8Size );
PORT_tenAPPReturns PORT_l_fenStuffingSTDMessage					( uint8 * pu8Var, uint8 u8Size, uint8 * pu8Target, uint8 * pu8StuffedSize );
PORT_tenAPPReturns PORT_l_fenCleanTheBuffer					( uint8 * pu8Var, uint8 u8Size );
PORT_tenAPPReturns PORT_l_fenStartDestuffing					( void );
PORT_tenAPPReturns PORT_l_fenDestuffingSTDMessage				( uint8 * pu8Var, uint8 u8Size, uint8 * pu8Target, uint8 * pu8StuffedSize );

uint8 PORT_l_au8STDParsingBits[CAN_STD_MAX];
uint8 PORT_l_au8STDStuffedBits[CAN_STD_MAX + CAN_STD_MAX/MAX_STUFFING + 0x01];

uint8 PORT_l_u8SizeOfSTDMessageInBits;
uint8 PORT_l_u8SizeOfSTDStuffedInBits;

static PORT_tenAPPReturns PORT_l_fvDummyCRC(uint16* u16CRC)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  if(NULL != u16CRC)
  {
    *u16CRC = (PORT_l_stStdCANMessage.unSOF.bfSOF.bfSOF0 << MASK_SHIFT_SOF)      |
        (PORT_l_stStdCANMessage.unIDF.u16IDF << MASK_SHIFT_IDF) |
        (PORT_l_stStdCANMessage.unRTR.u8RTR << MASK_SHIFT_RTR)      |
        (PORT_l_stStdCANMessage.unIDE.u8IDE << MASK_SHIFT_IDE)      |
        (PORT_l_stStdCANMessage.unRB0.u8RB0 << MASK_SHIFT_RB0);
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
 *                       This routine handles the initialization for the CAN Stuffing.
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
void PORT_g_fvCANStuffingInit(void)
{
  PORT_tenAPPReturns enReturnCallCRC;

  PORT_l_u8Payload[CAN_PAYLOAD_BYTE0] = 0x00;
  PORT_l_u8Payload[CAN_PAYLOAD_BYTE1] = 0xFF;
  PORT_l_u8Payload[CAN_PAYLOAD_BYTE2] = 0xAA;
  PORT_l_u8Payload[CAN_PAYLOAD_BYTE3] = 0x55;

  PORT_l_stStdCANMessage.unSOF.u8SOF = CAN_SOF_VALUE;
  PORT_l_stStdCANMessage.unIDF.u16IDF = (uint16)0x55AAu;
  PORT_l_stStdCANMessage.unRTR.u8RTR = CAN_DEFAULT_REG_VAL;
  PORT_l_stStdCANMessage.unIDE.u8IDE = CAN_DEFAULT_REG_VAL;
  PORT_l_stStdCANMessage.unRB0.u8RB0 = CAN_DEFAULT_REG_VAL;
  PORT_l_stStdCANMessage.unDLC.u8DLC = 0x04u;
  PORT_l_stStdCANMessage.au8DataPayload = &PORT_l_u8Payload[0];

  enReturnCallCRC = PORT_l_fvDummyCRC(&PORT_l_stStdCANMessage.u16CRC);

  DDM_DBG("\r\n CRC Stuffing Calculation");

  PORT_l_u8SizeOfSTDMessageInBits = CLEAR_VALUE_1B;

  PORT_g_fvDecideTheReturn(enReturnCallCRC);
  PORT_l_enCANStuffingState = hPORT_nenCANStuffingStart;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine is the handler for the CAN Stuffing.
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
void PORT_g_fvCANStuffingHandler(void)
{
  switch(PORT_l_enCANStuffingState)
  {
    case hPORT_nenCANStuffingStart:
    {
      PORT_l_enCANStuffingState = hPORT_nenCANStuffingParsing;
      break;
    }

    case hPORT_nenCANStuffingParsing:
    {
      if( hPORT_nenSuccess == PORT_l_fenParseSTDMessage(&PORT_l_stStdCANMessage))
      {
        PORT_l_enCANStuffingState = hPORT_nenCANStuffingPrintingParsedObtained;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }
      break;
    }

    case hPORT_nenCANStuffingPrintingParsedObtained:
    {
      if( hPORT_nenSuccess == PORT_l_fenPrintBitsOfMessage( &PORT_l_au8STDParsingBits[0], PORT_l_u8SizeOfSTDMessageInBits ))
      {
        PORT_l_enCANStuffingState = hPORT_nenCANStuffingStep;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }

      break;
    }

    case hPORT_nenCANStuffingStep:
    {
      if( hPORT_nenSuccess == PORT_l_fenStuffingSTDMessage( &PORT_l_au8STDParsingBits[0], PORT_l_u8SizeOfSTDMessageInBits, &PORT_l_au8STDStuffedBits[0], &PORT_l_u8SizeOfSTDStuffedInBits ))
      {
        PORT_l_enCANStuffingState = hPORT_nenCANStuffedPrintStep;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }

      break;
    }

    case hPORT_nenCANStuffedPrintStep:
    {
      if( hPORT_nenSuccess == PORT_l_fenPrintBitsOfMessage( &PORT_l_au8STDStuffedBits[0], PORT_l_u8SizeOfSTDStuffedInBits ))
      {
        PORT_l_enCANStuffingState = hPORT_nenCANStuffedStartDestuff;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }

      break;
    }

    case hPORT_nenCANStuffedStartDestuff:
    {
      if( hPORT_nenSuccess == PORT_l_fenStartDestuffing() )
      {
        /* Success to start de-stuffing bits mechanism! */
        PORT_l_enCANStuffingState = hPORT_nenCANStuffingTotalSteps;
      }
      else
      {
        /* Failed to start de-stuffing bits mechanism! */
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
 *                       This routine represents the stop for the CAN Stuffing.
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
void PORT_g_fvCANStuffingSTOP(void)
{

}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the CAN Destuffing.
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
void PORT_g_fvCANDestuffingInit(void)
{
  PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffingIDLE;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine is the handler for the CAN Destuffing.
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
void PORT_g_fvCANDestuffingHandler(void)
{
  switch(PORT_l_enCANDestuffingSteps)
  {
    case hPORT_nenCANDestuffingIDLE:
    {

      break;
    }
    case hPORT_nenCANDestuffingStart:
    {
      if( hPORT_nenSuccess == PORT_l_fenCleanTheBuffer(&PORT_l_au8STDParsingBits[0], CAN_STD_MAX))
      {
        PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffingStep;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }

      break;
    }

    case hPORT_nenCANDestuffingStep:
    {
      if( hPORT_nenSuccess == PORT_l_fenDestuffingSTDMessage(&PORT_l_au8STDStuffedBits[0], PORT_l_u8SizeOfSTDStuffedInBits, &PORT_l_au8STDParsingBits[0], &PORT_l_u8SizeOfSTDMessageInBits ))
      {
        PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffedPrintStep;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }

      break;
    }

    case hPORT_nenCANDestuffedPrintStep:
    {
      if( hPORT_nenSuccess == PORT_l_fenPrintBitsOfMessage( &PORT_l_au8STDParsingBits[0], PORT_l_u8SizeOfSTDMessageInBits ))
      {
        PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffedEnd;
      }
      else
      {
        /* We don't want to be here but we are. So bad. */
      }

      break;
    }

    case hPORT_nenCANDestuffedEnd:
    {
      PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffingIDLE;

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
 *                       This routine represents the stop for the CAN Destuffing.
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
 **
 * @test
 *                       Module Test Case IDs for traceability
 *******************************************************************************/
void PORT_g_fvCANDestuffingStop(void)
{

}


PORT_tenAPPReturns PORT_l_fenParseSTDMessage(PORT_tstStdCANMessage * stSTDMessage)
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8Index = 0x00;
  uint8 u8IndexArray = 0x00;

  PORT_l_au8STDParsingBits[u8Index] =  (stSTDMessage->unSOF.bfSOF.bfSOF0) & SET_VALUE_1B; // SOF Bit;
  u8Index++;

  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF0)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit0;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF1)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit1;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF2)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit2;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF3)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit3;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF4)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit4;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF5)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit5;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF6)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit6;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF7)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit7;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF8)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit8;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF9)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit9;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  ((stSTDMessage->unIDF.bfIDF.bfIDF10)>>MASK_SHIFT_BYTE_0) & SET_VALUE_1B; // IDF Bit10;
  u8Index++;

  PORT_l_au8STDParsingBits[u8Index] =  (stSTDMessage->unRTR.bfRTR.bfRTR0) & SET_VALUE_1B; // RTR Bit;
  u8Index++;

  PORT_l_au8STDParsingBits[u8Index] =  (stSTDMessage->unIDE.bfIDE.bfIDE0) & SET_VALUE_1B; // IDE Bit;
  u8Index++;

  PORT_l_au8STDParsingBits[u8Index] =  (stSTDMessage->unRB0.bfRB0.bfRB0_0) & SET_VALUE_1B; // RB0 Bit;
  u8Index++;

  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->unDLC.bfDLC.bfDLC0 >> MASK_SHIFT_BYTE_0)) & SET_VALUE_1B; // DLC Bit0;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->unDLC.bfDLC.bfDLC1 >> MASK_SHIFT_BYTE_0)) & SET_VALUE_1B; // DLC Bit1;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->unDLC.bfDLC.bfDLC2 >> MASK_SHIFT_BYTE_0)) & SET_VALUE_1B; // DLC Bit2;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->unDLC.bfDLC.bfDLC3 >> MASK_SHIFT_BYTE_0)) & SET_VALUE_1B; // DLC Bit3;
  u8Index++;

  for(u8IndexArray = 0x00u; u8IndexArray < stSTDMessage->unDLC.u8DLC; u8IndexArray++)
  {
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_7)) & SET_VALUE_1B; // Payload Bit7;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_6)) & SET_VALUE_1B; // Payload Bit6;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_5)) & SET_VALUE_1B; // Payload Bit5;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_4)) & SET_VALUE_1B; // Payload Bit4;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_3)) & SET_VALUE_1B; // Payload Bit3;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_2)) & SET_VALUE_1B; // Payload Bit2;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_1)) & SET_VALUE_1B; // Payload Bit1;
    u8Index++;
    PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->au8DataPayload[u8IndexArray] >> MASK_SHIFT_BYTE_0)) & SET_VALUE_1B; // Payload Bit0;
    u8Index++;
  }

  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_14)) & SET_VALUE_1B; // CRC Bit14;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_13)) & SET_VALUE_1B; // CRC Bit13;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_12)) & SET_VALUE_1B; // CRC Bit12;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_11)) & SET_VALUE_1B; // CRC Bit11;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_10)) & SET_VALUE_1B; // CRC Bit10;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_9)) & SET_VALUE_1B; // CRC Bit9;

  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_8)) & SET_VALUE_1B; // CRC Bit8;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_7)) & SET_VALUE_1B; // CRC Bit7;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_6)) & SET_VALUE_1B; // CRC Bit6;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_5)) & SET_VALUE_1B; // CRC Bit5;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_4)) & SET_VALUE_1B; // CRC Bit4;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_3)) & SET_VALUE_1B; // CRC Bit3;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_2)) & SET_VALUE_1B; // CRC Bit2;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_1)) & SET_VALUE_1B; // CRC Bit1;
  u8Index++;
  PORT_l_au8STDParsingBits[u8Index] =  (uint8)((stSTDMessage->u16CRC >> MASK_SHIFT_BYTE_0)) & SET_VALUE_1B; // CRC Bit0;
  u8Index++;

  PORT_l_u8SizeOfSTDMessageInBits = u8Index;
  enRetCode = hPORT_nenSuccess;

  return enRetCode;
}

PORT_tenAPPReturns PORT_l_fenPrintBitsOfMessage( uint8 * pu8Var, uint8 u8Size )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8Index;
  DDM_DBG("\r\n Print on bits: \n\r");

  for(u8Index = CLEAR_VALUE_1B; u8Index < u8Size; u8Index++)
  {
#if PORT_PRINTS_ENABLED == SET_VALUE_1B
    printf("%X ", pu8Var[u8Index]);
    fflush(stdout);
#endif
  }

  enRetCode = hPORT_nenSuccess;

  return enRetCode;
}

PORT_tenAPPReturns PORT_l_fenStuffingSTDMessage( uint8 * pu8Var, uint8 u8Size, uint8 * pu8Target, uint8 * pu8StuffedSize )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8IndexOrig, u8IndexStuffed;
  uint8 u8Count = CLEAR_VALUE_1B;
  uint8 u8LastVal;

  pu8Target[CLEAR_VALUE_1B] = pu8Var[CLEAR_VALUE_1B];
  u8LastVal = pu8Var[CLEAR_VALUE_1B];
  u8Count++;
  u8IndexStuffed = CLEAR_VALUE_1B + 0x01u;
  for(u8IndexOrig = CLEAR_VALUE_1B + 0x01u; u8IndexOrig < u8Size; u8IndexOrig++)
  {
    if(u8LastVal == pu8Var[u8IndexOrig])
    {
      u8Count++;

      if( MAX_STUFFING ==  u8Count )
      {
        pu8Target[u8IndexStuffed] = pu8Var[u8IndexOrig];
        u8IndexStuffed++;
        pu8Target[u8IndexStuffed] = !(u8LastVal&SET_VALUE_1B);
        u8LastVal = !(u8LastVal&SET_VALUE_1B);
        u8Count = 0x01;
        u8IndexStuffed++;
      }
      else
      {
        pu8Target[u8IndexStuffed] = pu8Var[u8IndexOrig];
        u8IndexStuffed++;
      }
    }
    else
    {
      u8LastVal = pu8Var[u8IndexOrig];
      u8Count = SET_VALUE_1B;
      pu8Target[u8IndexStuffed] = pu8Var[u8IndexOrig];
      u8IndexStuffed++;
    }
  }

  *pu8StuffedSize = u8IndexStuffed;

  enRetCode = hPORT_nenSuccess;

  return enRetCode;
}

PORT_tenAPPReturns PORT_l_fenCleanTheBuffer( uint8 * pu8Var, uint8 u8Size )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8Index;

  for(u8Index = CLEAR_VALUE_1B; u8Index < u8Size; u8Index++)
  {
    pu8Var[u8Index] = CLEAR_VALUE_1B;
  }
  DDM_DBG("\r\n Cleared the buffer!");

  enRetCode = hPORT_nenSuccess;

  return enRetCode;
}

PORT_tenAPPReturns PORT_l_fenStartDestuffing( void )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;

  PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffingStart;

  enRetCode = hPORT_nenSuccess;
  return enRetCode;
}

PORT_tenAPPReturns PORT_l_fenDestuffingSTDMessage( uint8 * pu8Var, uint8 u8Size, uint8 * pu8Target, uint8 * pu8StuffedSize )
{
  PORT_tenAPPReturns enRetCode = hPORT_nenUndefinedReturn;
  uint8 u8IndexStuffed, u8IndexDestuffed;
  uint8 u8Count = CLEAR_VALUE_1B;
  uint8 u8LastVal;

  pu8Target[CLEAR_VALUE_1B] = pu8Var[CLEAR_VALUE_1B];
  u8LastVal = pu8Var[CLEAR_VALUE_1B];
  u8Count++;
  u8IndexStuffed = CLEAR_VALUE_1B + NEXT_BUFFER_ELEMENT;
  u8IndexDestuffed = CLEAR_VALUE_1B + NEXT_BUFFER_ELEMENT;

  for(u8IndexStuffed = CLEAR_VALUE_1B + NEXT_BUFFER_ELEMENT; u8IndexStuffed < u8Size; u8IndexStuffed++, u8IndexDestuffed++)
  {
    if( u8LastVal != pu8Var[u8IndexStuffed] )
    {
      u8LastVal = pu8Var[u8IndexStuffed];
      u8Count = SET_VALUE_1B;
      pu8Target[u8IndexDestuffed] = pu8Var[u8IndexStuffed];
    }
    else
    {
      u8Count++;
      if( MAX_STUFFING == u8Count )
      {
        pu8Target[u8IndexDestuffed] = pu8Var[u8IndexStuffed];
        u8IndexStuffed++;
        u8LastVal = pu8Var[u8IndexStuffed];
        if( pu8Var[u8IndexStuffed + NEXT_BUFFER_ELEMENT] == u8LastVal )
        {
          u8Count = SET_VALUE_1B;
        }
        else
        {
          u8Count = CLEAR_VALUE_1B;
        }
      }
      else
      {
        pu8Target[u8IndexDestuffed] = pu8Var[u8IndexStuffed];
      }
    }
  }

  *pu8StuffedSize = u8IndexDestuffed;

  enRetCode = hPORT_nenSuccess;

  return enRetCode;
}

/******************************************************************************
 * EOF: CAN_Stuffing.c
 *******************************************************************************/
