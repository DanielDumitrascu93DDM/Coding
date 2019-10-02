/*
 * DDM 27/09/2019
 *  */

#ifndef _CAN_CAN_STUFFING_T_H_
#define _CAN_CAN_STUFFING_T_H_


//===============================================================================
//      Class Own Header file
//===============================================================================
#include "../defines.h"

#define CAN_PADDING             0xAAu

#define FLOW_BS_OFFSET          0x01u
#define FLOW_STMIN_OFFSET       0x02u


#define CAN_STD_PAYLOAD         0x08u
#define CAN_SOF_VALUE           0x00u
#define CAN_DEFAULT_REG_VAL     0x00u
#define CAN_PAYLOAD_BYTE0       0x00u
#define CAN_PAYLOAD_BYTE1       0x01u
#define CAN_PAYLOAD_BYTE2       0x02u
#define CAN_PAYLOAD_BYTE3       0x03u
#define CAN_PAYLOAD_BYTE4       0x04u
#define CAN_PAYLOAD_BYTE5       0x05u
#define CAN_PAYLOAD_BYTE6       0x06u
#define CAN_PAYLOAD_BYTE7       0x07u

#define MASK_SHIFT_SOF          0x0Eu
#define MASK_SHIFT_IDF          0x03u
#define MASK_SHIFT_RTR          0x02u
#define MASK_SHIFT_IDE          0x01u
#define MASK_SHIFT_RB0          0x00u

#define MASK_SHIFT_BYTE_0          0x00u
#define MASK_SHIFT_BYTE_1          0x01u
#define MASK_SHIFT_BYTE_2          0x02u
#define MASK_SHIFT_BYTE_3          0x03u
#define MASK_SHIFT_BYTE_4          0x04u
#define MASK_SHIFT_BYTE_5          0x05u
#define MASK_SHIFT_BYTE_6          0x06u
#define MASK_SHIFT_BYTE_7          0x07u
#define MASK_SHIFT_BYTE_8          0x08u
#define MASK_SHIFT_BYTE_9          0x09u
#define MASK_SHIFT_BYTE_10         0x0Au
#define MASK_SHIFT_BYTE_11         0x0Bu
#define MASK_SHIFT_BYTE_12         0x0Cu
#define MASK_SHIFT_BYTE_13         0x0Du
#define MASK_SHIFT_BYTE_14         0x0Eu
#define MASK_SHIFT_BYTE_15         0x0Fu

#define CAN_STD_MAX             0x5Eu

#define CLEAR_VALUE_1B			0x00u
#define CLEAR_VALUE_2B			0x0000u
#define CLEAR_VALUE_4B			0x00000000u

#define SET_VALUE_1B			0x01u
#define MAX_STUFFING			0x05u

#define NEXT_BUFFER_ELEMENT     0x01u

typedef struct
{
  uint8     bfSOF0       :1;
  uint8     reserved    :7;

}PORT_tbfSOF;

typedef union
{
  uint8         u8SOF;
  PORT_tbfSOF   bfSOF;

}PORT_tunSOF;

typedef struct
{
#if BIG_ENDIAN == ENABLE
  uint16     bfIDF10      :1;
  uint16     bfIDF9       :1;
  uint16     bfIDF8       :1;
  uint16     bfIDF7       :1;
  uint16     bfIDF6       :1;
  uint16     bfIDF5       :1;
  uint16     bfIDF4       :1;
  uint16     bfIDF3       :1;
  uint16     bfIDF2       :1;
  uint16     bfIDF1       :1;
  uint16     bfIDF0       :1;
#else
  uint16     bfIDF0      :1;
  uint16     bfIDF1       :1;
  uint16     bfIDF2       :1;
  uint16     bfIDF3       :1;
  uint16     bfIDF4       :1;
  uint16     bfIDF5       :1;
  uint16     bfIDF6       :1;
  uint16     bfIDF7       :1;
  uint16     bfIDF8       :1;
  uint16     bfIDF9       :1;
  uint16     bfIDF10       :1;
#endif
  uint8     reserved    :5;

}PORT_tbfIDF;

typedef union
{
  uint16         u16IDF;
  PORT_tbfIDF    bfIDF;

}PORT_tunIDF;

typedef struct
{
  uint8     bfRTR0       :1;
  uint8     reserved    :7;

}PORT_tbfRTR;

typedef union
{
  uint8         u8RTR;
  PORT_tbfRTR   bfRTR;

}PORT_tunRTR;

typedef struct
{
  uint8     bfIDE0      :1;
  uint8     reserved    :7;

}PORT_tbfIDE;

typedef union
{
  uint8         u8IDE;
  PORT_tbfIDE   bfIDE;

}PORT_tunIDE;

typedef struct
{
  uint8     bfRB0_0     :1;
  uint8     reserved    :7;

}PORT_tbfRB0;

typedef union
{
  uint8         u8RB0;
  PORT_tbfRB0   bfRB0;

}PORT_tunRB0;

typedef struct
{
#if BIG_ENDIAN == ENABLE

  uint8     bfDLC3       :1;
  uint8     bfDLC2       :1;
  uint8     bfDLC1       :1;
  uint8     bfDLC0       :1;
#else
  uint8     bfDLC0       :1;
  uint8     bfDLC1       :1;
  uint8     bfDLC2       :1;
  uint8     bfDLC3       :1;
#endif
  uint8     reserved    :4;

}PORT_tbfDLC;

typedef union
{
  uint8          u8DLC;
  PORT_tbfDLC    bfDLC;

}PORT_tunDLC;

typedef struct
{
  PORT_tunSOF     unSOF;
  PORT_tunIDF     unIDF;
  PORT_tunRTR     unRTR;
  PORT_tunIDE     unIDE;
  PORT_tunRB0     unRB0;
  PORT_tunDLC     unDLC;
  uint8     *au8DataPayload;
  uint16    u16CRC      ;

}PORT_tstStdCANMessage;


#endif // _CAN_CAN_STUFFING_T_H_
/******************************************************************************
 * EOF: CAN_Stuffing_t.h
 *******************************************************************************/
