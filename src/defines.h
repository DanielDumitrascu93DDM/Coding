/*
 * DDM 27/09/2019
 *  */

#ifndef _DEFINES_H_
#define _DEFINES_H_

//===============================================================================
//      Class Own Header file
//===============================================================================

typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned long  int  uint32;

#define MAX_ALLOWED_CALLS ((uint16)0x00FF)
#define BIG_ENDIAN ENABLE
#define NULL 0x00u

#define PORT_PRINTS_ENABLED 0x01u
#define ENABLE	 	    0x01u
#define DISABLE 		    0x00u

#define ONE_BYTE_DEFAULT 0x00u
#define TWO_BYTES_DEFAULT 0x0000u
#define FOUR_BYTES_DEFAULT 0x0000u

#if PORT_PRINTS_ENABLED == ENABLE
#define DDM_DBG(sString)\
    printf((const unsigned char*)sString); \
    fflush (stdout)
#else
#define DDM_DBG(sString)\
    printf(""); \
    fflush (stdout)
#endif

#define clrscr() printf("\n\n\n\n\n\n\n\n\n\n");\
		 printf("\n\n\n\n\n\n\n\n\n\n");\
		 printf("\n\n\n\n\n\n\n\n\n\n");\
		 printf("\n\n\n\n\n\n\n\n\n\n");\
		 printf("\n\n\n\n\n\n\n\n\n\n");


typedef enum
{

  hPORT_nenSuccess           = 0x00u,    /* Success operation return */
  hPORT_nenFailure           ,           /* Failure operation return */
  hPORT_nenBusy              ,           /* Busy operation return, try next time */
  hPORT_nenNULLPointer       ,           /* NULL pointer return */
  hPORT_nenInvalidParameter  ,           /* Invalid Parameter(s) operation return */
  hPORT_nenUndefinedReturn   ,           /* An undefined return, use it as init value. */
  hPORT_nenFullReturn   ,           /* An undefined return, use it as init value. */

  hPORT_nenTotalReturns

}PORT_tenAPPReturns;

#endif // _DEFINES_H_
/******************************************************************************
 * EOF: defines.h
 *******************************************************************************/
