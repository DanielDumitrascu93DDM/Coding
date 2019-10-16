/*
 * LIN.c
 *
 *  Created on: Oct 14, 2019
 *      Author: danieldumitrascu
 */
#include "LIN_i.h"

#define BIG_ENDIAN ENABLE

static PORT_tstLINFrame PORT_l_stLINFrame;
static uint8 PORT_l_au8Buffer[MAX_SIZE_LIN_BUFFER];
static PORT_tenLINSm PORT_l_enLINSm;

static void PORT_l_fvDecideHighLow(uint8 u8Val);
static void PORT_g_fvPrintLINBreak(uint16 u16BreakVal);
static void PORT_g_fvPrintLINSync(uint8 u8Sync);
static void PORT_g_fvPrintLINId(uint8 u8Id);
static void PORT_l_fvDecideDataSizeToBeSent(uint8 u8Id);
static void PORT_l_fvDecideLINFrameType(uint8 u8Id);

static uint8 PORT_l_au8LINIds[MAX_SIZE_LIN_IDS] = {4, 33, 44, 62, 63};

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the LIN.
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
void PORT_g_fvLINInit(void)
{
	PORT_l_stLINFrame.unLINBreak.u16Break = TWO_BYTES_DEFAULT;
	PORT_l_stLINFrame.unLINSync.u8Sync = ONE_BYTE_DEFAULT;
	PORT_l_stLINFrame.unLINId.u8Id = ONE_BYTE_DEFAULT;

	PORT_l_enLINSm = PORT_nenLINPopulateBreak;
}

/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the LIN.
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
void PORT_g_fvLINHandler(void)
{
	static uint8 u8Index;

	switch(PORT_l_enLINSm)
	{
		case PORT_nenLINPopulateBreak:
		{
			  /* 1 1 1 1 1 1 0 0 1 1 0 0 0 1 1 0 0 1 0 1 0 1 0 1 */
			DDM_DBG("\r\n\n\n|         BREAK Section        |     SYNC      |    ID     |P P|");
			PORT_l_stLINFrame.unLINBreak.u16Break = LIN_FRAME_TEST_BREAK;
			PORT_g_fvPrintLINBreak(PORT_l_stLINFrame.unLINBreak.u16Break);
			PORT_l_enLINSm = PORT_nenLINPopulateSync;

			break;
		}

		case PORT_nenLINPopulateSync:
		{
			PORT_l_stLINFrame.unLINSync.u8Sync = LIN_FRAME_TEST_SYNC;
			PORT_g_fvPrintLINSync(PORT_l_stLINFrame.unLINSync.u8Sync);
			PORT_l_enLINSm = PORT_nenLINPopulateId;

			break;
		}

		case PORT_nenLINPopulateId:
		{

			PORT_l_stLINFrame.unLINId.u8Id = PORT_l_au8LINIds[u8Index];
			PORT_l_stLINFrame.unLINId.u8Id = PORT_l_stLINFrame.unLINId.u8Id<<0x02;
			//Pack the priority.
			PORT_l_stLINFrame.unLINId.stLINId.bfLINParity0 = 0x01;
			PORT_l_stLINFrame.unLINId.stLINId.bfLINParity1 = 0x01;
			PORT_g_fvPrintLINId(PORT_l_stLINFrame.unLINId.u8Id);
			PORT_l_fvDecideLINFrameType(PORT_l_stLINFrame.unLINId.u8Id);
			PORT_l_fvDecideDataSizeToBeSent(PORT_l_stLINFrame.unLINId.u8Id );
			u8Index++;

			if( MAX_SIZE_LIN_IDS <= u8Index )
			{
				PORT_l_enLINSm = PORT_nenLINDataLength;
			}
			else
			{
				PORT_l_enLINSm = PORT_nenLINPopulateBreak;
			}

			break;
		}

		case PORT_nenLINDataLength:
		{
			PORT_l_enLINSm = PORT_nenLINFrameType;

			break;
		}

		case PORT_nenLINFrameType:
		{
			PORT_l_enLINSm = PORT_nenLINFrameFormat;

			break;
		}

		case PORT_nenLINFrameFormat:
		{
			DDM_DBG("\r\n Done LIN Header Learning.\r\n");
			PORT_l_enLINSm = PORT_nenLINTotalStates;

			break;
		}

		default:
		{
			break;
		}
	}
}


void PORT_g_fvPrintLINBreak(uint16 u16BreakVal)
{
	PORT_tunLINBreak unLINBreak;
	unLINBreak.u16Break = u16BreakVal;
	DDM_DBG("\n");

#if BIG_ENDIAN == ENABLE
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak0);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak1);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak2);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak3);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak4);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak5);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak6);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak7);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak8);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak9);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak10);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak11);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak12);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak13);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak14);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak15);
#else
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak15);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak14);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak13);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak12);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak11);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak10);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak9);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak8);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak7);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak6);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak5);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak4);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak3);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak2);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak1);
	PORT_l_fvDecideHighLow(unLINBreak.stLINBreak.bfLINBreak0);
#endif
}

static void PORT_g_fvPrintLINSync(uint8 u8Sync)
{
	PORT_tunLINSync unLINSync;
	unLINSync.u8Sync = u8Sync;
#if BIG_ENDIAN == ENABLE
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync0);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync1);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync2);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync3);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync4);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync5);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync6);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync7);
#else

	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync7);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync6);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync5);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync4);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync3);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync2);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync1);
	PORT_l_fvDecideHighLow(unLINSync.stLINSync.bfLINSync0);
#endif
}

static void PORT_g_fvPrintLINId(uint8 u8Id)
{
	PORT_tunLINId unLINId;
	unLINId.u8Id = u8Id;

#if BIG_ENDIAN == ENABLE
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId0);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId1);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId2);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId3);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId4);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId5);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINParity0);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINParity1);
#else
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINParity1);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINParity0);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId5);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId4);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId3);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId2);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId1);
	PORT_l_fvDecideHighLow(unLINId.stLINId.bfLINId0);
#endif
}


static void PORT_l_fvDecideHighLow(uint8 u8Val)
{
	if(u8Val)
	{
		DDM_DBG("1 ");
	}
	else
	{
		DDM_DBG("0 ");
	}
}

static void PORT_l_fvDecideDataSizeToBeSent(uint8 u8Id)
{
	uint8 u8LocalVal;
	u8LocalVal = (uint8)((u8Id & ((uint8)LIN_ID_MASK))>>0x02);

	if ( ( u8LocalVal >= LIN_ID_2BYTES_LOW ) && (u8LocalVal <= LIN_ID_2BYTES_HIGH ) )
	{
		DDM_DBG("\r\n 2 bytes response expected.");
	}
	else
	{
		if ( ( u8LocalVal >= LIN_ID_4BYTES_LOW ) && (u8LocalVal <= LIN_ID_4BYTES_HIGH ) )
		{
			DDM_DBG("\r\n 4 bytes response expected.");
		}
		else
		{
			if ( ( u8LocalVal >= LIN_ID_8BYTES_LOW ) && (u8LocalVal <= LIN_ID_8BYTES_HIGH ) )
			{
				DDM_DBG("\r\n 8 bytes response expected.");
			}
			else
			{
				DDM_DBG("\r\n Unknown size response expected.");
			}
		}
	}
}


static void PORT_l_fvDecideLINFrameType(uint8 u8Id)
{
	uint8 u8LocalVal;
	u8LocalVal = (uint8)((u8Id & ((uint8)LIN_ID_MASK))>>0x02);

	if ( ( u8LocalVal >= LIN_UNCONDITIONAL_LOW ) && (u8LocalVal <= LIN_UNCONDITIONAL_HIGH ) )
	{
		DDM_DBG("\r\n LIN Unconditional | Event Triggered | Sporadic Frame Type.");
	}
	else
	{
		if ( ( u8LocalVal >= LIN_DIAGNOSTIC_LOW ) && (u8LocalVal <= LIN_DIAGNOSTIC_HIGH) )
		{
			DDM_DBG("\r\n Diagnostic Frame Type.");
		}
		else
		{
			if (u8LocalVal == LIN_USER)
			{
				DDM_DBG("\r\n User Frame Type.");
			}
			else
			{
				if (u8LocalVal == LIN_USER_RESERVED)
				{
					DDM_DBG("\r\n Reserved Frame Type.");
				}
				else
				{
					DDM_DBG("\r\n Unknown Frame Type.");
				}			}
		}
	}
}



/******************************************************************************
 * FILE GLOBAL FUNCTIONS
 *******************************************************************************/
/**
 * @brief
 *                       This routine handles the initialization for the LIN.
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
void PORT_g_fvLINStop(void)
{
	/* TODO */
}
