/*
 * DDM 01/10/2019
 *  */

#ifndef _KLINE_KLINE_DEFINES_T_H_
#define _KLINE_KLINE_DEFINES_T_H_

//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "../ForOthers.h"

#define KLINE_ADDRESS           0x33u
#define KLINE_SYNC              0x55u

#define KLINE_KEY_BYTE1         0x3Fu
#define KLINE_KEY_BYTE2         0x8Fu

#define KLINE_BUFFER_MAX        0x0Au
#define BUFFER_FIRST_ELEMENT    0x00u
#define BUFFER_SECOND_ELEMENT   0x01u
#define BUFFER_ONE_ELEMENT      0x01u
#define BUFFER_TWO_ELEMENTs     0x02u

typedef enum
{

  hPORT_nenKlineTSTIdle           = 0x00u,    /* Success operation return */
  hPORT_nenKlineTSTSendAddress,
  hPORT_nenKlineTSTRecvSyncByte,
  hPORT_nenKlineTSTRecvKeyBytes,
  hPORT_nenKlineTSTSendKeyByte2Inv,
  hPORT_nenKlineTSTRecvAddressInv,

  hPORT_nenKlineTSTSMTotalSteps
}PORT_tenKlineTesterSM;

typedef enum
{

  hPORT_nenKlineECUIdle           = 0x00u,    /* Success operation return */
  hPORT_nenKlineECURecvAddress,
  hPORT_nenKlineECUSendSyncByte,
  hPORT_nenKlineECUSendKeyBytes,
  hPORT_nenKlineECURecvKeyByte2Inv,
  hPORT_nenKlineECUSendAddressInv,

  hPORT_nenKlineECUSMTotalSteps
}PORT_tenKlineECUSM;

typedef struct
{
  PORT_tenKlineTesterSM     enTSTActualStateMachine      ;
  PORT_tenKlineTesterSM     enTSTOldStateMachine         ;
  PORT_tenKlineECUSM        enECUActualStateMachine      ;
  PORT_tenKlineECUSM        enECUOldStateMachine         ;
  uint8                     * au8RXBuffer;
  uint8                     u8IndexTX;
  uint8                     u8IndexRX;

}PORT_tstKline;

#endif // _KLINE_KLINE_DEFINES_T_H_
/******************************************************************************
 * EOF: Kline_defines_t.h
 *******************************************************************************/
