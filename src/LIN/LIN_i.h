/*
 * LIN_i.h
 *
 *  Created on: Oct 14, 2019
 *      Author: danieldumitrascu
 */

#ifndef LIN_LIN_I_H_
#define LIN_LIN_I_H_


#include "LIN_e.h"
#include "../ForOthers.h"

#define LIN_FRAME_TEST_BREAK 0xFCC6u
#define LIN_FRAME_TEST_SYNC 0x55u
#define LIN_ID_MASK 0xFCu

#define LIN_ID_2BYTES_LOW  0x00
#define LIN_ID_2BYTES_HIGH 0x1F
#define LIN_ID_4BYTES_LOW  0x20
#define LIN_ID_4BYTES_HIGH 0x2F
#define LIN_ID_8BYTES_LOW  0x30
#define LIN_ID_8BYTES_HIGH 0x3F

#define LIN_UNCONDITIONAL_LOW  	0x00
#define LIN_UNCONDITIONAL_HIGH  0x3B
#define LIN_DIAGNOSTIC_LOW  	0x3C
#define LIN_DIAGNOSTIC_HIGH  	0x3D
#define LIN_USER 				0x3E
#define LIN_USER_RESERVED 		0x3F

#endif /* LIN_LIN_I_H_ */
