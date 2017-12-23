/*  modbustypes.h - data to monitor APC USB/HID devices with NUT
*
*  Copyright (C)
*      2017            Dmitry Togushev <jtprofacc@gmain.com>
*
*      Based on ModbusComm.h apcupsd (http://www.apcupsd.org/)
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*/

/*
*	Implements HID over USB and MODBUS over USB protocols for Schneider Electric SMT and SRT family models.
*	Check
*	APPLICATION NOTE #178 (http://www.apc.com/salestools/MPAO-99NQF6/MPAO-99NQF6_R1_EN.pdf)
*	APPLICATION NOTE #176 (http://www.apc.com/salestools/MPAO-98KJ7F/MPAO-98KJ7F_R1_EN.pdf)
*	for more information
*/

#ifndef MODBUSTYPES_H
#define MODBUSTYPES_H

//#ifdef __cplusplus
/* *INDENT-OFF* */
//extern "C" {
/* *INDENT-ON* */
//#endif /* __cplusplus */

#include <sys/types.h>

#include "nut_stdint.h"
#include <time.h>

#include <stdbool.h>
#include <usb.h>

// MODBUS message sizes
#define MODBUS_MAX_FRAME_SZ 256
#define MODBUS_MAX_PDU_SZ  MODBUS_MAX_FRAME_SZ - 4

#define  MODBUS_USB_REPORT_SIZE 64
#define  MODBUS_USB_REPORT_MAX_FRAME_SIZE  MODBUS_USB_REPORT_SIZE - 1

// MODBUS timeouts
static const unsigned int MODBUS_INTERCHAR_TIMEOUT_MS = 25; // Spec is 15, increase for compatibility with USB serial dongles
static const unsigned int MODBUS_INTERFRAME_TIMEOUT_MS = 45; // Spec is 35, increase due to UPS missing messages occasionally
static const unsigned int MODBUS_IDLE_WAIT_TIMEOUT_MS = 100;
static const unsigned int MODBUS_RESPONSE_TIMEOUT_MS = 500;

// MODBUS function codes
static const uint8_t MODBUS_FC_ERROR = 0x80;
static const uint8_t MODBUS_FC_READ_HOLDING_REGS = 0x03;
static const uint8_t MODBUS_FC_WRITE_REG = 0x06;
static const uint8_t MODBUS_FC_WRITE_MULTIPLE_REGS = 0x10;


typedef enum {
	DT_BITFIELD,
	DT_ENUMERATION,
	DT_BINARYPOINT_S0,
	DT_BINARYPOINT_S1,
	DT_BINARYPOINT_S2,
	DT_BINARYPOINT_S3,
	DT_BINARYPOINT_S4,
	DT_BINARYPOINT_S5,
	DT_BINARYPOINT_S6,
	DT_BINARYPOINT_S7,
	DT_BINARYPOINT_S8,
	DT_BINARYPOINT_S9,
	DT_BINARYPOINT_U0,
	DT_BINARYPOINT_U1,
	DT_BINARYPOINT_U2,
	DT_BINARYPOINT_U3,
	DT_BINARYPOINT_U4,
	DT_BINARYPOINT_U5,
	DT_BINARYPOINT_U6,
	DT_BINARYPOINT_U7,
	DT_BINARYPOINT_U8,
	DT_BINARYPOINT_U9,
	DT_STRING
} MB_DataType;

bool scaleDT(MB_DataType dt, uint8_t *scale);
int decodeDT(MB_DataType dt);

uint64_t BE2LE(uint8_t *reg, uint8_t sz);
void LE2BE(uint64_t val, uint8_t *reg, uint8_t sz);


// Dates in APC MODBUS registers are expressed as number of days since 
// 1/1/2000. This constant represents 1/1/2000 (UTC) as UNIX timestamp.
static const time_t MODBUS_BASE_TIMESTAMP = 946684800;

//static const unsigned int MODBUS_MAX_FRAME_SZ = 256;
//static const unsigned int MODBUS_MAX_PDU_SZ = MODBUS_MAX_FRAME_SZ - 4;

typedef uint8_t ModbusFrame[MODBUS_MAX_FRAME_SZ];
typedef uint8_t ModbusPdu[MODBUS_MAX_PDU_SZ];

typedef usb_dev_handle *		usb_dev_handle_t;

#define TV_DIFF_MS(a, b) (((b).tv_sec - (a).tv_sec) * 1000 + ((b).tv_usec - (a).tv_usec) / 1000) 

#define BIN8(x) BIN___(0##x)
#define BIN___(x)                                        \
	(                                                \
	((x / 01ul) % 010)*(2>>1) +                      \
	((x / 010ul) % 010)*(2<<0) +                     \
	((x / 0100ul) % 010)*(2<<1) +                    \
	((x / 01000ul) % 010)*(2<<2) +                   \
	((x / 010000ul) % 010)*(2<<3) +                  \
	((x / 0100000ul) % 010)*(2<<4) +                 \
	((x / 01000000ul) % 010)*(2<<5) +                \
	((x / 010000000ul) % 010)*(2<<6)                 \
	)

#define BIN16(x1,x2) \
    ((BIN(x1)<<8)+BIN(x2))

#define BEBIN16(x1,x2) \
    ((BIN(x2)<<8)+BIN(x1))

#endif //MODBUSTYPES_H

