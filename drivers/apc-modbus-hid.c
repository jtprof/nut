/*  apc-modbus-hid.c - data to monitor APC USB/HID devices with NUT
*
*  Copyright (C)
*      2017            Dmitry Togushev <jtprofacc@gmain.com>
*
*      Based on apc-hid.c NUT (http://networkupstools.org/)
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

#include <stdbool.h>
#include <stdint.h>

#include "main.h"     /* for getval() */
#include "usbhid-ups.h"
#include "apc-modbus-hid.h"
#include "usb-common.h"
#include "smtmodbus.h"
#include "libmodbus.h"
#include "modbustypes.h"
#include <math.h>

#define APC_HID_VERSION "APC MODBUS over HID 0.1"

#define APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL 2

/* APC */
#define APC_VENDORID 0x051d
#define UPS_OUTLET_GROUP_NUM 2

int get_UPS_outlets_group_num() {
	//TODO Check and analyze register 590 I_SOGRelayConfigSetting_BF to find out real configuration when the register will return corret value 
	return UPS_OUTLET_GROUP_NUM;
}

int CheckModbusEnable(usb_dev_handle_t udev, USBDevice_t *hd) {
	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::CheckModbusEnable...");
	uint16_t regnum = 0;
	uint8_t *reg = ModbusReadRegister(udev, hd->MODBUSID, regnum, 2);
	if (reg == NULL) {
		return 0;
	}
	free(reg);
	return 1;
}

int MBGetDataValue(hid_dev_handle_t udev, hid_info_t *hidups_item, double *Value, int age) {
	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBGetDataValue. reg=%u, nregs=%u, NUTCMD: %s, datatype = %u", hidups_item->mbregID, hidups_item->mbregLEN, hidups_item->info_type, hidups_item->datatype);
	uint8_t *reg = ModbusReadRegister(udev, DEFAULT_SLAVE_ADDR, hidups_item->mbregID, hidups_item->mbregLEN);
	upsdebug_hex(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBGetDataValue. ModbusReadRegister return buffer", reg, hidups_item->mbregLEN * sizeof(uint16_t));

	if (reg == NULL) {
		return 0;
	}
	switch (decodeDT(hidups_item->datatype)) {
		case 1: {			//DT_BINARYPOINT
			uint8_t nbytes = hidups_item->mbregLEN * sizeof(uint16_t);
			uint64_t v = BE2LE(reg, nbytes);
			uint8_t scale = 9;

			upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBGetDataValue. DT_BINARYPOINT");
			if (scaleDT(hidups_item->datatype, &scale))
			{
				upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBGetDataValue. DT_BINARYPOINT. Signed. scale= %u", scale);
				// Sign extend
				int64_t sint = v;
				sint <<= (8 * (sizeof(v) - nbytes));
				sint >>= (8 * (sizeof(v) - nbytes));
				*Value = (double)sint / (1ULL << scale);
			}
			else
			{
				upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBGetDataValue. DT_BINARYPOINT. UnSigned. scale= %u", scale);
				*Value = (double)v / (1ULL << scale);
			}
			break; 
		}
		case 2: break;		//DT_ENUMERATION
		case 3: break;		//DT_STRING
		case 0:				//DT_BITFIELD
		default: {
			uint64_t v = BE2LE(reg, hidups_item->mbregLEN * sizeof(uint16_t));
			upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBGetDataValue. DT_BITFIELD. value= 0x%08x", v);
			*Value = (double)v;
		}
	}
	free(reg);
	return 1;
}

int MBSetDataValue(hid_dev_handle_t udev, hid_info_t	*hidups_item, double Value) {

	uint8_t data[sizeof(uint64_t)] = { 0 };
	uint8_t nbytes = hidups_item->mbregLEN * sizeof(uint16_t);

	switch (decodeDT(hidups_item->datatype)) {
		case 1: {			//DT_BINARYPOINT
			uint8_t scale = 9;
			uint64_t v;

			upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBSetDataValue. DT_BINARYPOINT. Value = %f", Value);
			if (scaleDT(hidups_item->datatype, &scale))
			{
				if (scale == 0) {
					v = (uint64_t)ceil(Value);
				}
				else {
					v = (uint64_t)ceil(Value*(1ULL << scale) + 0.5);
				}
				upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBSetDataValue. DT_BINARYPOINT. v = %d (0x%08x)", v, v);
				int64_t sint = v;
				sint <<= (8 * (sizeof(v) - nbytes));
				sint >>= (8 * (sizeof(v) - nbytes));
				v = sint;
			}
			else {
				v = (uint64_t)ceil(Value*(1ULL << scale) + 0.5);
			}

			LE2BE(v, data, nbytes);

			upsdebug_hex(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::MBSetDataValue. data to write: ", &data, sizeof(uint64_t));

			if (ModbusWriteRegister(udev, DEFAULT_SLAVE_ADDR, hidups_item->mbregID, hidups_item->mbregLEN, data)) {
				return 1;
			}
		}
		case 2: break;		//DT_ENUMERATION
		case 3: break;		//DT_STRING
		case 0:				//DT_BITFIELD
		default: {
			LE2BE(hidups_item->Command, data, nbytes);
			if (ModbusWriteRegister(udev, DEFAULT_SLAVE_ADDR, hidups_item->mbregID, hidups_item->mbregLEN, data)) {
				return 1;
			}

		}
	}
	return 0;
}

/* Don't use interrupt pipe on 5G models (used by proprietary protocol) */
static void *disable_interrupt_pipe(USBDevice_t *device)
{
	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::disable_interrupt_pipe...");
	if (use_interrupt_pipe == TRUE) {
		upslogx(LOG_INFO, "interrupt pipe disabled (add 'pollonly' flag to 'ups.conf' to get rid of this message)");
		use_interrupt_pipe= FALSE;
	}
	return NULL;
}

static const char *apc_format_model(HIDDevice_t *hd) {
	static char model[64];
	char *ptr1, *ptr2;

	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::apc_format_model...");

	/* FIXME?: what is the path "UPS.APC_UPS_FirmwareRevision"? */
	snprintf(model, sizeof(model), "%s", hd->Product ? hd->Product : "unknown");
	ptr1 = strstr(model, "FW:");
	if (ptr1)
	{
		*(ptr1 - 1) = '\0';
		ptr1 += strlen("FW:");
		ptr2 = strstr(ptr1, "USB FW:");
		if (ptr2)
		{
			*(ptr2 - 1) = '\0';
			ptr2 += strlen("USB FW:");
			dstate_setinfo("ups.firmware.aux", "%s", ptr2);
		}
		dstate_setinfo("ups.firmware", "%s", ptr1);
	}
	return model;
}

static const char *apc_format_mfr(HIDDevice_t *hd) {
	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::apc_format_mfr...");
	return hd->Vendor ? hd->Vendor : "APC";
}

static const char *apc_format_serial(HIDDevice_t *hd) {
	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::apc_format_serial...");
	return hd->Serial;
}

static usb_device_id_t apc_usb_device_table[] = {
	{ USB_DEVICE(APC_VENDORID, 0x0003), disable_interrupt_pipe },
	/* Terminating entry */
	{ -1, -1, NULL }
};

int apc_claim(HIDDevice_t *hd) {

	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::apc_claim...");

	hd->MODBUSID = DEFAULT_SLAVE_ADDR;   //TODO!!!


	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "\thd->VendorID: %d\thd->ProductID: %d\thd->Vendor: %s\thd->Product: %s\thd->Serial: %s\thd->Bus: %s\thd->bcdDevice: %d\thd->MODBUSID: %d", hd->VendorID, hd->ProductID, hd->Vendor, hd->Product, hd->Serial, hd->Bus, hd->bcdDevice, hd->MODBUSID);

	int status = is_usb_device_supported(apc_usb_device_table, hd);
	
	switch (status) {

		case POSSIBLY_SUPPORTED:
			/* by default, reject, unless the productid option is given */
			if (getval("productid")) {
				return 1;
			}
			possibly_supported("APC", hd);
			return 0;

		case SUPPORTED:
			return 1;

		case NOT_SUPPORTED:
		default:
			return 0;
	}
}


subdriver_t apc_modbus_hid_subdriver = {
	APC_HID_VERSION,
	apc_claim,
	apc_utab,
	apc_hid2nut,
	apc_format_model,
	apc_format_mfr,
	apc_format_serial,
};

/*

static const char *apc_modbus_hid_runtime_calibration_status_fun(double value) {
uint64_t v = (uint64_t)value;
//static char buf[20] = {0};

upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::apc_modbus_hid_runtime_calibration_status_fun... value = %f, v= 0x%8x, Pending = %d, InProgress = %d, Passed = %d", value, v, Pending, InProgress, Passed);
if ((smtStatusAndCommand_BF)v & Pending) return "Pending";
if ((smtStatusAndCommand_BF)v & InProgress) return "In progress";
if ((smtStatusAndCommand_BF)v & Passed) return "Done and passed";
if ((smtStatusAndCommand_BF)v & Failed) return "Done and error";
if ((smtStatusAndCommand_BF)v & Refused) return "No test initiated";
if ((smtStatusAndCommand_BF)v & Aborted) return "Aborted";

}

// * returns statically allocated string - must not use it again before
// done with result! 
// 
static const char *apc_modbus_hid_date_conversion_fun(double value)
{
	upsdebugx(APC_MODBUS_HID_MESSAGES_DEBUG_LEVEL, "apc-hid::apc_date_conversion_fun...");
	static char buf[20];
	int year, month, day;

	if ((long)value == 0) {
		return "not set";
	}

	/* APC apparently uses a hexadecimal-as-decimal format, e.g., 0x102202 = October 22, 2002 
	year = ((long)value & 0xf) + 10 * (((long)value >> 4) & 0xf);
	month = (((long)value >> 16) & 0xf) + 10 * (((long)value >> 20) & 0xf);
	day = (((long)value >> 8) & 0xf) + 10 * (((long)value >> 12) & 0xf);

	/* Y2K conversion - hope that this format will be retired before 2070 :) 
	if (year >= 70) {
		year += 1900;
	}
	else {
		year += 2000;
	}

	snprintf(buf, sizeof(buf), "%04d/%02d/%02d", year, month, day);

	return buf;
}

static const char *apc_modbus_hid_outlets_status_conversion_fun(double value) {
uint64_t v = (uint64_t)value;
//static char buf[20] = {0};

if (v & BIN8(00000001)) return "on";
if (v & BIN8(00000010)) return "off";
//	if (v & BIN8(00000010)) return "off";
}

info_lkp_t apc_modbus_hid_date_conversion[] = {
{ 0, NULL, apc_modbus_hid_date_conversion_fun }
};

info_lkp_t apc_modbus_hid_outlets_status_conversion[] = {
{ 0, NULL, apc_modbus_hid_outlets_status_conversion_fun }
};

info_lkp_t apc_modbus_hid_runtime_calibration_status[] = {
{ 0, NULL, apc_modbus_hid_runtime_calibration_status_fun }
};




// */