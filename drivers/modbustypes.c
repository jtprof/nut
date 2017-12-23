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

#include <sys/types.h>
#include <stdint.h>

#include "nut_stdint.h"
#include <time.h>

#include <stdbool.h>
#include "modbustypes.h"

int decodeDT(MB_DataType dt) {
	switch (dt) {
		case DT_BINARYPOINT_S0: return 1;
		case DT_BINARYPOINT_S1: return 1;
		case DT_BINARYPOINT_S2: return 1;
		case DT_BINARYPOINT_S3: return 1;
		case DT_BINARYPOINT_S4: return 1;
		case DT_BINARYPOINT_S5: return 1;
		case DT_BINARYPOINT_S6: return 1;
		case DT_BINARYPOINT_S7: return 1;
		case DT_BINARYPOINT_S8: return 1;
		case DT_BINARYPOINT_S9: return 1;
		case DT_BINARYPOINT_U0: return 1;
		case DT_BINARYPOINT_U1: return 1;
		case DT_BINARYPOINT_U2: return 1;
		case DT_BINARYPOINT_U3: return 1;
		case DT_BINARYPOINT_U4: return 1;
		case DT_BINARYPOINT_U5: return 1;
		case DT_BINARYPOINT_U6: return 1;
		case DT_BINARYPOINT_U7: return 1;
		case DT_BINARYPOINT_U8: return 1;
		case DT_BINARYPOINT_U9: return 1; 
		case DT_ENUMERATION: return 2;
		case DT_STRING: return 3;
		case DT_BITFIELD:
		default: return 0;
	}
}

bool scaleDT(MB_DataType dt, uint8_t *scale) {
	switch (dt) {
		case DT_BINARYPOINT_S0: {*scale = 0; return true;}
		case DT_BINARYPOINT_S1: {*scale = 1; return true;}
		case DT_BINARYPOINT_S2: {*scale = 2; return true;}
		case DT_BINARYPOINT_S3: {*scale = 3; return true;}
		case DT_BINARYPOINT_S4: {*scale = 4; return true;}
		case DT_BINARYPOINT_S5: {*scale = 5; return true;}
		case DT_BINARYPOINT_S6: {*scale = 6; return true; }
		case DT_BINARYPOINT_S7: {*scale = 7; return true;}
		case DT_BINARYPOINT_S8: {*scale = 8; return true;}
		case DT_BINARYPOINT_S9: {*scale = 9; return true; }
		case DT_BINARYPOINT_U0: {*scale = 0; return false;}
		case DT_BINARYPOINT_U1: {*scale = 1; return false;}
		case DT_BINARYPOINT_U2: {*scale = 2; return false;}
		case DT_BINARYPOINT_U3: {*scale = 3; return false;}
		case DT_BINARYPOINT_U4: {*scale = 4; return false;}
		case DT_BINARYPOINT_U5: {*scale = 5; return false;}
		case DT_BINARYPOINT_U6: {*scale = 6; return false;}
		case DT_BINARYPOINT_U7: {*scale = 7; return false;}
		case DT_BINARYPOINT_U8: {*scale = 8; return false;}
		case DT_BINARYPOINT_U9: {*scale = 9; return false;}
		case DT_STRING:
		case DT_BITFIELD:
		case DT_ENUMERATION: {*scale = 0; return false; }

	}
}

uint64_t BE2LE(uint8_t *reg, uint8_t sz) {
	uint64_t value = 0; int i;
	for (i = 0; i < sz ; i++) {
		value = value << 8;
		value = value | reg[i];
	}
	return value;
}

void LE2BE(uint64_t val, uint8_t *reg, uint8_t sz) {
	unsigned int i;
	for (i = 0; i < sz; ++i)
		reg[i] = val >> (8 * (sz - i - 1));
}
