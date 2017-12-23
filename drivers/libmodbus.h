/*  libmodbus.h - data to monitor APC USB/HID devices with NUT
*
*  Copyright (C)
*      2017            Dmitry Togushev <jtprofacc@gmain.com>
*
*      Based on ModbusUsbComm.h apcupsd (http://www.apcupsd.org/)
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

#ifndef _LIBMODBUS_H
#define _LIBMODBUS_H

#include "config.h"

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

#include "modbustypes.h"
//#include "usb-common.h"
#ifdef MODBUS_OVER_HID
#include <usb.h>
#endif


// MODBUS constants
#define DEFAULT_SLAVE_ADDR 1

#define ModbusRTURx 0xFF8600FC
#define ModbusRTUTx 0xFF8600FD

//#define ModbusHIDRxID 0x90
//#define ModbusHIDTxID 0x89

#define ModbusHIDRxID 0x89
#define ModbusHIDTxID 0x90
#define EP_Rx 0x81
#define EP_Tx 0x01

#define LIBUSB_ENDPOINT_IN  0x80
#define LIBUSB_ENDPOINT_OUT 0x00

#ifdef MODBUS_OVER_HID
typedef usb_dev_handle *		modbus_dev_handle_t;
#endif


/* ---------------------------------------------------------------------- */

uint8_t *ModbusReadRegister(modbus_dev_handle_t udev, uint8_t modbusID, uint16_t reg, unsigned int nregs);
bool ModbusWriteRegister(modbus_dev_handle_t udev, uint8_t modbusID, uint16_t reg, unsigned int nregs, const uint8_t *data);

#endif /* _LIBMODBUS_H */
