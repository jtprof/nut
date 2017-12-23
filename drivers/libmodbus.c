/*  libmodbus.c - data to monitor APC USB/HID devices with NUT
*
*  Copyright (C)
*      2017            Dmitry Togushev <jtprofacc@gmain.com>
*
*      Based on ModbusUsbComm.c apcupsd (http://www.apcupsd.org/)
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* #include <math.h> */
#include "libmodbus.h"
#include "modbustypes.h"
#include "common.h" /* for xmalloc, upsdebugx prototypes */
#include "usb-common.h"

#ifdef MODBUS_OVER_HID
#include "libusb.h"
#endif

#define MODBUS_MESSAGES_DEBUG_LEVEL 2

// Support functions. Headers
bool CommModbusTx(modbus_dev_handle_t udev, const ModbusFrame *frm, unsigned int sz);
bool CommModbusRx(modbus_dev_handle_t udev, ModbusFrame *frm, unsigned int *sz);
uint16_t ModbusCrc(const uint8_t *data, unsigned int sz);
bool CommWaitIdle(modbus_dev_handle_t udev); bool ModbusWaitIdle();
bool ModbusSendAndWait(modbus_dev_handle_t udev, uint8_t modbusID, uint8_t fc, const ModbusPdu *txpdu, unsigned int txsz, ModbusPdu *rxpdu, unsigned int rxsz);

time_t ModbusRegTotime_t(uint64_t reg);
uint64_t time_tToModbusReg(time_t t);

// Main functions.
uint8_t *ModbusReadRegister(modbus_dev_handle_t udev, uint8_t modbusID, uint16_t reg, unsigned int nregs)
{
   ModbusPdu txpdu;
   ModbusPdu rxpdu;
   const unsigned int nbytes = nregs * sizeof(uint16_t);

    upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusReadRegister. reg=%u, nregs=%u", reg, nregs);

   txpdu[0] = reg >> 8;
   txpdu[1] = reg;
   txpdu[2] = nregs >> 8;
   txpdu[3] = nregs;

   if (!ModbusSendAndWait(udev, modbusID, MODBUS_FC_READ_HOLDING_REGS, &txpdu, 4,
                    &rxpdu, nbytes+1))
   {
	   upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusReadRegister. Error during read register.");
	   return NULL;
   }

   if (rxpdu[0] != nbytes)
   {
      // Invalid size
	   upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusReadRegister.  Wrong number of data bytes received (exp=%u, rx=%u)", nbytes, rxpdu[0]);
   }

   uint8_t *ret = calloc(nbytes, sizeof(uint8_t)); //new uint8_t[nbytes];
   if (!ret) {
	   return NULL;
   }
   memcpy(ret, rxpdu+1, nbytes);
   return ret;
}

bool ModbusWriteRegister(modbus_dev_handle_t udev, uint8_t modbusID, uint16_t reg, unsigned int nregs, const uint8_t *data)
{
   ModbusPdu txpdu;
   ModbusPdu rxpdu;
   const unsigned int nbytes = nregs * sizeof(uint16_t);

   upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusWriteRegister. reg=%u, nregs=%u", reg, nregs);

   txpdu[0] = reg >> 8;
   txpdu[1] = reg;
   txpdu[2] = nregs >> 8;
   txpdu[3] = nregs;
   txpdu[4] = nbytes;
   memcpy(txpdu+5, data, nbytes);

   if (!ModbusSendAndWait(udev, modbusID, MODBUS_FC_WRITE_MULTIPLE_REGS, &txpdu, nbytes+5, &rxpdu, 4))
   {
      return false;
   }

   // Response should match first 4 bytes of command (reg and nregs)
   if (memcmp(rxpdu, txpdu, 4))
   {
      // Did not write expected number of registers
	  upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusWriteRegister. Write failed reg=%u, nregs=%u, writereg=%u, writenregs=%uu", reg, nregs, (rxpdu[0] << 8) | rxpdu[1], (rxpdu[2] << 8) | rxpdu[3]);
      return false;
   }

   return true;
}


// Support functions. Implementation

bool ModbusSendAndWait(modbus_dev_handle_t udev, uint8_t modbusID,
   uint8_t fc, 
   const ModbusPdu *txpdu, unsigned int txsz, 
   ModbusPdu *rxpdu, unsigned int rxsz)
{
   ModbusFrame txfrm;
   ModbusFrame rxfrm;
   unsigned int sz;

   upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. modbusID: 0x%04x, FC: 0x%04x ", modbusID, fc);
   upsdebug_hex(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. txpdu", txpdu, txsz);
   // Ensure caller isn't trying to send an oversized PDU
   if (txsz > MODBUS_MAX_PDU_SZ || rxsz > MODBUS_MAX_PDU_SZ)
      return false;

   // Prepend slave address and function code
   txfrm[0] = modbusID;//_slaveaddr;
   txfrm[1] = fc;

   // Add PDU
   memcpy(txfrm+2, txpdu, txsz);

   // Calculate crc
   uint16_t crc = ModbusCrc(txfrm, txsz+2);
   upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. CRC: 0x%04x ", crc);
   // CRC goes out LSB first, unlike other MODBUS fields
   txfrm[txsz+2] = crc;
   txfrm[txsz+3] = crc >> 8;

   int retries = 2;
   do
   {
	  upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Iteration %d.", 2 - retries);
	  if (!CommModbusTx(udev, &txfrm, txsz+4))
      {
         // Failure to send is immediately fatal
		 upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Failure to send is immediately fatal.");
         return false;
      }

      if (!CommModbusRx(udev, &rxfrm, &sz))
      {
         // Rx timeout: Retry
		 upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. CommModbusRx retutn FALSE.");
         continue;
      }

      if (sz < 4)
      {
         // Runt frame: Retry
		  upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Runt frame %u.", sz);
         continue;
      }

      crc = ModbusCrc(rxfrm, sz-2);
      if (rxfrm[sz-2] != (crc & 0xff) ||
          rxfrm[sz-1] != (crc >> 8))
      {
         // CRC error: Retry
		 upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. CRC error.");
         continue;
      }

      if (rxfrm[0] != modbusID) //_slaveaddr)
      {
         // Not from expected slave: Retry
         // TODO! change function Dmsg(0, "%s: Bad address (exp=%u, rx=%u)\n", __func__, _slaveaddr, rxfrm[0]);
		  upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Bad address (exp=%u, rx=%u).", modbusID, rxfrm[0]);
		  continue;
      }

      if (rxfrm[1] == (fc | MODBUS_FC_ERROR))
      {
         // Exception response: Immediately fatal
 		  upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Exception (code=%u).", modbusID, rxfrm[2]);
		  return false;
      }

      if (rxfrm[1] != fc)
      {
         // Unknown response: Retry
		  upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Unexpected response 0x%02x.", modbusID, rxfrm[1]);
		  continue;
      }

      if (sz != rxsz+4)
      {
         // Wrong size: Retry
		 upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Wrong size (exp=%u, rx=%u).", rxsz + 4, sz);
         continue;
      }

      // Everything is ok
      memcpy(rxpdu, rxfrm+2, rxsz);
      return true;
   }
   while (retries--);

   // Retries exhausted
   upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::ModbusSendAndWait. Retries exhausted");
   return false;
}

time_t ModbusRegTotime_t(uint64_t reg)
{
	return MODBUS_BASE_TIMESTAMP + reg * 60 * 60 * 24;
}

uint64_t time_tToModbusReg(time_t t)
{
	return (t - MODBUS_BASE_TIMESTAMP) / 60 / 60 / 24;
}

bool CommModbusTx(modbus_dev_handle_t udev, const ModbusFrame *frm, unsigned int sz)
{
	// MODBUS/USB is limited to 63 bytes of payload (we don't bother with
	// fragmentation/reassembly). Since we drop the CRC (last 2 bytes) because 
	// MODBUS/USB doesn't use it, frame size is 2 less than what caller says.

	int ret = 0;

	upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusTx...");
	if (sz - 2 > MODBUS_USB_REPORT_MAX_FRAME_SIZE) {
		upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusTx. sz - 2 > MODBUS_USB_REPORT_MAX_FRAME_SIZE.");
		return false;
	}

	// Wait for idle 
	if (!CommWaitIdle(udev)) {
		upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusTx. CommWaitIdle return FALSE.");
		return false;
	}

	upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusTx. Sending frame.");

	// We add HID report id as the first byte of the report, then at most 63
	// bytes of payload.
	uint8_t rpt[MODBUS_USB_REPORT_SIZE] = { 0 };
	rpt[0] = ModbusHIDTxID; // _txrpt;
	
	memcpy(rpt + 1, frm, sz - 2);
#ifdef MODBUS_OVER_HID
	ret = interrupt_write(udev, EP_Tx, &rpt, MODBUS_USB_REPORT_SIZE, MODBUS_RESPONSE_TIMEOUT_MS);
	
	upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusTx. interrupt_write processed %d bytes.", ret);
	upsdebug_hex(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusTx. interrupt_write return buffer", &rpt, MODBUS_USB_REPORT_SIZE);

#endif
	return ret == (int)MODBUS_USB_REPORT_SIZE;
}

bool CommModbusRx(modbus_dev_handle_t udev, ModbusFrame *frm, unsigned int *sz)
{
	struct timeval now, exittime;
	
	upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx...");

	// Determine time at which we need to exit
	gettimeofday(&exittime, NULL);
	exittime.tv_sec += MODBUS_RESPONSE_TIMEOUT_MS / 1000;
	exittime.tv_usec += (MODBUS_RESPONSE_TIMEOUT_MS % 1000) * 1000;
	if (exittime.tv_usec >= 1000000)
	{
		++exittime.tv_sec;
		exittime.tv_usec -= 1000000;
	}

	int ret = -ETIMEDOUT;
	uint8_t rpt[MODBUS_USB_REPORT_SIZE] = { 0 };
	while (1)
	{
		gettimeofday(&now, NULL);
		int timeout = TV_DIFF_MS(now, exittime);//NS_TO_MS(target - now)
#ifdef MODBUS_OVER_HID
		ret = interrupt_read(udev, EP_Rx, &rpt, MODBUS_USB_REPORT_SIZE, timeout);
#endif
		upsdebug_hex(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. interrupt_read returned buffer", &rpt, MODBUS_USB_REPORT_SIZE); 
		if (timeout <= 0 || ret == -ETIMEDOUT)
		{
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. interrupt_read return %d. TIMEOUT %d. ETIMEDOUT %d", ret, timeout, ETIMEDOUT);
			upsdebug_hex(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. interrupt_read return buffer", &rpt, MODBUS_USB_REPORT_SIZE);
			return false;
		}

		// Temporary failure
		if (ret == -EINTR || ret == -EAGAIN)
			continue;

		// Fatal error
		if (ret <= 0)
		{
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. Read error: %d", ret);
			return false;
		}

		// Filter out non-MODBUS reports
		if (rpt[0] != ModbusHIDRxID) // _rxrpt)
		{
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. Ignoring report id %x", rpt[0]);
			continue;
		}

		// Bad report size ... fatal
		if (ret != (int)MODBUS_USB_REPORT_SIZE)
		{
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. Bad size %d", ret);
			return false;
		}

		// We always get a full report containing MODBUS_USB_REPORT_MAX_FRAME_SIZE
		// bytes of data. Clip to actual size of live data by looking at the MODBUS 
		// PDU header. This is a blatant layering violation, but no way around it 
		// here. Which byte(s) we look at and how we calculate the length depends
		// on the opcode.
		unsigned frmsz;
		if (rpt[2] == MODBUS_FC_READ_HOLDING_REGS)
		{
			// READ_HOLDING_REGS response includes a size byte.
			// Add 3 bytes to PDU size to account for size byte itself 
			// plus frame header (slaveaddr and op code).
			frmsz = rpt[3] + 3;
		}
		else if (rpt[2] == MODBUS_FC_WRITE_MULTIPLE_REGS)
		{
			// WRITE_MULTIPLE_REGS response is always a fixed length
			// 2 byte frame header (slaveaddr and op code)
			// 2 byte register starting address
			// 2 byte register count
			frmsz = 6;
		}
		else
		{
			// Unsupported response message...we can't calculate its length
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx.  Unknown response type %x", rpt[2]);

			upsdebug_hex(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. interrupt_read return buffer", &rpt, MODBUS_USB_REPORT_SIZE);
			return false;
		}

		if (frmsz > MODBUS_USB_REPORT_MAX_FRAME_SIZE)
		{
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. Fragmented PDU received...not supported");
			return false;
		}

		// Copy data to caller's buffer. Live data starts after USB report id byte.
		memcpy(frm, rpt + 1, frmsz);

		// MODBUS/USB doesn't provide a CRC. 
		// Fill one in to make upper layer happy.
		uint16_t crc = ModbusCrc(*frm, frmsz);
		(*frm)[frmsz] = crc & 0xff;
		(*frm)[frmsz + 1] = crc >> 8;

		*sz = frmsz + 2;
		//upsdebug_hex(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommModbusRx. interrupt_read return buffer", &rpt, MODBUS_USB_REPORT_SIZE);
		return true;
	}
}

#define S_TO_NS(x)  ( (x) * 1000000000ULL )
#define MS_TO_NS(x) ( (x) * 1000000ULL )
#define US_TO_NS(x) ( (x) * 1000ULL )
#define NS_TO_MS(x) ( ((x)+999999) / 1000000ULL )

uint64_t CommGetTod()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return S_TO_NS(now.tv_sec) + US_TO_NS(now.tv_usec);
}

bool CommWaitIdle(modbus_dev_handle_t udev)
{
	upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommWaitIdle...");

	// Current TOD
	uint64_t now = CommGetTod();

	// When we will give up by
	uint64_t exittime = now + MS_TO_NS(MODBUS_IDLE_WAIT_TIMEOUT_MS);

	// Initial idle target
	uint64_t target = now + MS_TO_NS(MODBUS_INTERFRAME_TIMEOUT_MS);

	uint8_t rpt[MODBUS_USB_REPORT_SIZE];
	while (target <= exittime)
	{
		int rc; 		/* !!!!!!!!!
		_hidups.InterruptRead(USB_ENDPOINT_IN|1, rpt,
		MODBUS_USB_REPORT_SIZE, NS_TO_MS(target-now));
		//
		*/
#ifdef MODBUS_OVER_HID
		int timeout = NS_TO_MS(target - now); //TV_DIFF_MS(now, exittime);//
		if (timeout <= 0) { 
			timeout = 5;
		}
		rc = interrupt_read(udev, 1, &rpt, MODBUS_USB_REPORT_SIZE, timeout);
#endif
		if (rc == -ETIMEDOUT)
		{
			// timeout: line is now idle
			return true;
		}
		else if (rc <= 0 && rc != -EINTR && rc != -EAGAIN)
		{
			// fatal error
			// TODO! change function Dmsg(0, "%s: interrupt_read failed: %s\n", __func__, strerror(-rc));
			upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommWaitIdle. interrupt_read get result %d.", rc);
			return false;
		}

		// Refresh TOD
		now = CommGetTod();

		if (rc > 0)
		{
			if (rpt[0] == ModbusHIDRxID) //_rxrpt)
			{
				
				upsdebugx(MODBUS_MESSAGES_DEBUG_LEVEL, "libmodbus::CommWaitIdle. Out of sync");

				// Reset wait time
				target = now + MS_TO_NS(MODBUS_INTERFRAME_TIMEOUT_MS);
			}
			else
			{
				// Non-MODBUS reports are not an issue, just continue waiting
				// TODO! change function Dmsg(100, "%s: Non-MODBUS report id %u\n", __func__, rpt[0]);
			}
		}
	}

	return false;
}

uint16_t ModbusCrc(const uint8_t *data, unsigned int sz)
{
	// 1 + x^2 + x^15 + x^16
	static const uint16_t MODBUS_CRC_POLY = 0xA001;
	uint16_t crc = 0xffff;

	while (sz--)
	{
		crc ^= *data++;
		unsigned int i;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 0x1)
				crc = (crc >> 1) ^ MODBUS_CRC_POLY;
			else
				crc >>= 1;
		}
	}

	return crc;
}



