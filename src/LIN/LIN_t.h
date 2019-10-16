/*
 * LIN_t.h
 *
 *  Created on: Oct 14, 2019
 *      Author: danieldumitrascu
 */

#ifndef LIN_LIN_T_H_
#define LIN_LIN_T_H_

#include "../defines.h"
//#include "../ForOthers.h"

#define MAX_SIZE_LIN_BUFFER 0x0A
#define MAX_SIZE_LIN_IDS 0x05


typedef struct
{
#if BIG_ENDIAN == ENABLE
  uint16     bfLINBreak15      :1;
  uint16     bfLINBreak14      :1;
  uint16     bfLINBreak13      :1;
  uint16     bfLINBreak12      :1;
  uint16     bfLINBreak11      :1;
  uint16     bfLINBreak10      :1;
  uint16     bfLINBreak9       :1;
  uint16     bfLINBreak8       :1;
  uint16     bfLINBreak7       :1;
  uint16     bfLINBreak6       :1;
  uint16     bfLINBreak5       :1;
  uint16     bfLINBreak4       :1;
  uint16     bfLINBreak3       :1;
  uint16     bfLINBreak2       :1;
  uint16     bfLINBreak1       :1;
  uint16     bfLINBreak0       :1;
#else
  uint16     bfLINBreak0      :1;
  uint16     bfLINBreak1      :1;
  uint16     bfLINBreak2      :1;
  uint16     bfLINBreak3      :1;
  uint16     bfLINBreak4      :1;
  uint16     bfLINBreak5      :1;
  uint16     bfLINBreak6       :1;
  uint16     bfLINBreak7       :1;
  uint16     bfLINBreak8       :1;
  uint16     bfLINBreak9       :1;
  uint16     bfLINBreak10       :1;
  uint16     bfLINBreak11       :1;
  uint16     bfLINBreak12       :1;
  uint16     bfLINBreak13       :1;
  uint16     bfLINBreak14       :1;
  uint16     bfLINBreak15       :1;
#endif
}PORT_tstLINBreak;

typedef union
{
	uint16 u16Break;
	PORT_tstLINBreak stLINBreak;

}PORT_tunLINBreak;

typedef struct
{
#if BIG_ENDIAN == ENABLE
  uint8     bfLINSync7       :1;
  uint8     bfLINSync6       :1;
  uint8     bfLINSync5       :1;
  uint8     bfLINSync4       :1;
  uint8     bfLINSync3       :1;
  uint8     bfLINSync2       :1;
  uint8     bfLINSync1       :1;
  uint8     bfLINSync0       :1;
#else
  uint8     bfLINSync0       :1;
  uint8     bfLINSync1       :1;
  uint8     bfLINSync2       :1;
  uint8     bfLINSync3       :1;
  uint8     bfLINSync4       :1;
  uint8     bfLINSync5       :1;
  uint8     bfLINSync6       :1;
  uint8     bfLINSync7       :1;
#endif
}PORT_tstLINSync;

typedef union
{
	uint8 u8Sync;
	PORT_tstLINSync stLINSync;

}PORT_tunLINSync;

typedef struct
{
#if BIG_ENDIAN == ENABLE
  uint8     bfLINParity1       :1;
  uint8     bfLINParity0       :1;
  uint8     bfLINId5       :1;
  uint8     bfLINId4       :1;
  uint8     bfLINId3       :1;
  uint8     bfLINId2       :1;
  uint8     bfLINId1       :1;
  uint8     bfLINId0       :1;
#else
  uint8     bfLINId0       :1;
  uint8     bfLINId1       :1;
  uint8     bfLINId2       :1;
  uint8     bfLINId3       :1;
  uint8     bfLINId4       :1;
  uint8     bfLINId5       :1;
  uint8     bfLINParity0       :1;
  uint8     bfLINParity1       :1;
#endif
}PORT_tstLINId;

typedef union
{
	uint8 u8Id;
	PORT_tstLINId stLINId;

}PORT_tunLINId;

typedef struct
{
	PORT_tunLINBreak unLINBreak;
	PORT_tunLINSync unLINSync;
	PORT_tunLINId unLINId;
	uint8 * au8Payload;
	uint8 u8Size;
}PORT_tstLINFrame;

typedef enum
{
	PORT_nenLINPopulateBreak = 0x00,
	PORT_nenLINPopulateSync		,
	PORT_nenLINPopulateId		,
	PORT_nenLINDataLength		,
	PORT_nenLINFrameType		,
	PORT_nenLINFrameFormat		,

	PORT_nenLINTotalStates
}PORT_tenLINSm;

#endif /* LIN_LIN_T_H_ */
