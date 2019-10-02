/*
 * DDM 27/09/2019
 *  */

#ifndef _CAN_CAN_STUFFING_I_H_
#define _CAN_CAN_STUFFING_I_H_


//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"
#include "../ForOthers.h"
#include "CAN_Stuffing_e.h"

typedef enum
{

  hPORT_nenCANStuffingStart           = 0x00u,    /* Success operation return */
  hPORT_nenCANStuffingParsing,
  hPORT_nenCANStuffingPrintingParsedObtained,
  hPORT_nenCANStuffingStep,
  hPORT_nenCANStuffedPrintStep,
  hPORT_nenCANStuffedStartDestuff,

  hPORT_nenCANStuffingTotalSteps
}PORT_tenCANStuffingSteps;

typedef enum
{

  hPORT_nenCANDestuffingStart           = 0x00u,    /* Success operation return */
  hPORT_nenCANDestuffingStep,
  hPORT_nenCANDestuffedPrintStep,
  hPORT_nenCANDestuffingIDLE,
  hPORT_nenCANDestuffedEnd,

  hPORT_nenCANDestuffingTotalSteps
}PORT_tenCANDestuffingSteps;


static uint8 PORT_l_u8Payload[CAN_STD_PAYLOAD];
static PORT_tstStdCANMessage PORT_l_stStdCANMessage;

static PORT_tenCANStuffingSteps PORT_l_enCANStuffingState = hPORT_nenCANStuffingStart;
static PORT_tenCANDestuffingSteps PORT_l_enCANDestuffingSteps = hPORT_nenCANDestuffingIDLE;

#endif // _CAN_CAN_STUFFING_I_H_
/******************************************************************************
 * EOF: CAN_Stuffing_i.h
 *******************************************************************************/
