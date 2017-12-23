/*  apc-modbus-hid.h - data to monitor APC USB/HID devices with NUT
*
*  Copyright (C)
*      2017            Dmitry Togushev <jtprofacc@gmain.com>
*
*      Based on apc-hid.h NUT (http://networkupstools.org/)
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

#ifndef APC_HID_H
#define APC_HID_H

#include "usbhid-ups.h"
#include "modbustypes.h"
#include "usb-common.h"
#include "smtmodbus.h"


extern subdriver_t apc_modbus_hid_subdriver;

extern int max_report_size;
int MBGetDataValue(hid_dev_handle_t udev, hid_info_t *hidups_item, double *Value, int age);
int MBSetDataValue(hid_dev_handle_t udev, hid_info_t	*hidups_item, double Value);
int CheckModbusEnable(usb_dev_handle_t udev, USBDevice_t *hd);
int get_UPS_outlets_group_num();


/* --------------------------------------------------------------- */
/*      Vendor-specific usage table */
/* --------------------------------------------------------------- */

/* APC usage table */
static usage_lkp_t apc_usage_lkp[] = {
/*MODBUS over HID Implementation in APC Smart-UPS http://www.apc.com/salestools/MPAO-98KJ7F/MPAO-98KJ7F_R1_EN.pdf */
	{ "APCModbusRTURx",	0xff8600fc },
	{ "APCModbusRTUTx",	0xff8600fd },

	{ NULL, 0 }
};

static usage_tables_t apc_utab[] = {
	apc_usage_lkp,
	hid_usage_lkp,
	NULL,
};

//extern info_lkp_t apc_modbus_hid_date_conversion[];
//extern info_lkp_t apc_modbus_hid_outlets_status_conversion[];
//extern info_lkp_t apc_modbus_hid_runtime_calibration_status[];

/* This was determined empirically from observing a BackUPS LS 500 */
static info_lkp_t apcstatusflag_info[] = {
	{ 8, "!off", NULL },  /* Normal operation */
	{ 16, "!off", NULL }, /* This occurs briefly during power-on, and corresponds to status 'DISCHRG'. */
	{ 0, "off", NULL },
	{ 0, NULL, NULL }
};

/* Reason of the last battery transfer (from apcupsd) */
static info_lkp_t apc_linefailcause_vrange_info[] = {
	{ 1, "vrange", NULL },	/* Low line voltage */
	{ 2, "vrange", NULL },	/* High line voltage */
	{ 4, "vrange", NULL },	/* notch, spike, or blackout */
	{ 8, "vrange", NULL },	/* Notch or blackout */
	{ 9, "vrange", NULL },	/* Spike or blackout */
	{ 0, "!vrange", NULL },		/* No transfers have ocurred */
	{ 0, NULL, NULL }
};

static info_lkp_t apc_linefailcause_frange_info[] = {
	{ 7, "frange", NULL },		/* Input frequency out of range */
	{ 0, "!frange", NULL },		/* No transfers have ocurred */
	{ 0, NULL, NULL }
};

static info_lkp_t apc_sensitivity_info[] = {
	{ 0, "low", NULL },
	{ 1, "medium", NULL },
	{ 2, "high", NULL },
	{ 0, NULL, NULL }
};

/* --------------------------------------------------------------- */
/*      HID2NUT lookup table                                       */
/* --------------------------------------------------------------- */

/*
0.078634	[D1] Path: UPS.PowerSummary.iProduct, Type: Feature, ReportID: 0x01, Offset: 0, Size: 8, Value: 2
0.085719	[D1] Path: UPS.PowerSummary.iSerialNumber, Type: Feature, ReportID: 0x02, Offset: 0, Size: 8, Value: 3
0.091940	[D1] Path: UPS.PowerSummary.iManufacturer, Type: Feature, ReportID: 0x03, Offset: 0, Size: 8, Value: 1
0.098061	[D1] Path: UPS.PowerSummary.iOEMInformation, Type: Feature, ReportID: 0x04, Offset: 0, Size: 8, Value: 1
0.112780	[D1] Path: UPS.PowerSummary.Rechargeable, Type: Feature, ReportID: 0x06, Offset: 0, Size: 8, Value: 1
0.112809	[D1] Path: UPS.PowerSummary.CapacityMode, Type: Feature, ReportID: 0x06, Offset: 8, Size: 8, Value: 2
0.119899	[D1] Path: UPS.PowerSummary.DesignCapacity, Type: Feature, ReportID: 0x0e, Offset: 0, Size: 8, Value: 100
0.119923	[D1] Path: UPS.PowerSummary.FullChargeCapacity, Type: Feature, ReportID: 0x0e, Offset: 8, Size: 8, Value: 100
0.134605	[D1] Path: UPS.PowerSummary.CapacityGranularity1, Type: Feature, ReportID: 0x10, Offset: 0, Size: 8, Value: 1
0.134630	[D1] Path: UPS.PowerSummary.CapacityGranularity2, Type: Feature, ReportID: 0x10, Offset: 8, Size: 8, Value: 1
0.181769	[D1] Path: UPS.PowerSummary.ConfigVoltage, Type: Feature, ReportID: 0x0a, Offset: 0, Size: 16, Value: 36
0.202956	[D1] Path: UPS.PowerSummary.PresentStatus.ShutdownRequested, Type: Input, ReportID: 0x07, Offset: 5, Size: 1, Value: 0
0.202969	[D1] Path: UPS.PowerSummary.PresentStatus.ShutdownRequested, Type: Feature, ReportID: 0x07, Offset: 5, Size: 1, Value: 0
0.203036	[D1] Path: UPS.PowerSummary.PresentStatus.CommunicationLost, Type: Input, ReportID: 0x07, Offset: 8, Size: 1, Value: 0
0.203049	[D1] Path: UPS.PowerSummary.PresentStatus.CommunicationLost, Type: Feature, ReportID: 0x07, Offset: 8, Size: 1, Value: 0
0.203117	[D1] Path: UPS.PowerSummary.PresentStatus.VoltageNotRegulated, Type: Input, ReportID: 0x07, Offset: 11, Size: 1, Value: 0
0.203130	[D1] Path: UPS.PowerSummary.PresentStatus.VoltageNotRegulated, Type: Feature, ReportID: 0x07, Offset: 11, Size: 1, Value: 0
// */

/* HID2NUT lookup table */
static hid_info_t apc_hid2nut[] = {
	{ "battery.type", 0, 0, "UPS.PowerSummary.iDeviceChemistry", NULL, "%s", 0, stringid_conversion },
	{ "battery.charge", 0, 0, "UPS.PowerSummary.RemainingCapacity", NULL, "%.0f", 0, NULL },
	{ "battery.charge.low", ST_FLAG_RW | ST_FLAG_STRING, 10, "UPS.PowerSummary.RemainingCapacityLimit", NULL, "%.0f", HU_FLAG_SEMI_STATIC, NULL },
	{ "battery.charge.warning", 0, 0, "UPS.PowerSummary.WarningCapacityLimit", NULL, "%.0f", 0, NULL },
	{ "ups.mfr.date", 0, 0, "UPS.PowerSummary.ManufacturerDate", NULL, "%s", 0, date_conversion }, 
	{ "battery.runtime", 0, 0, "UPS.PowerSummary.RunTimeToEmpty", NULL, "%.0f", 0, NULL },
	//{ "ups.delay.shutdown", ST_FLAG_RW | ST_FLAG_STRING, 10, "UPS.PowerSummary.DelayBeforeShutdown", NULL, DEFAULT_OFFDELAY, HU_FLAG_ABSENT, NULL },
	//{ "ups.timer.shutdown", 0, 0, "UPS.PowerSummary.DelayBeforeShutdown", NULL, "%.0f", HU_FLAG_QUICK_POLL, NULL },
	{ "ups.timer.reboot", 0, 0, "UPS.PowerSummary.DelayBeforeReboot", NULL, "%.0f", HU_FLAG_QUICK_POLL, NULL },
	{ "battery.runtime.low", ST_FLAG_RW | ST_FLAG_STRING, 10, "UPS.PowerSummary.RemainingTimeLimit", NULL, "%.0f", HU_FLAG_SEMI_STATIC, NULL },
	{ "battery.voltage.nominal", 0, 0, "UPS.PowerSummary.ConfigVoltage", NULL, "%.1f", 0, NULL }, 
	{ "battery.voltage",  0, 0, "UPS.PowerSummary.Voltage", NULL, "%.1f", 0, NULL },
	{ "beeper.on", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "2", HU_TYPE_CMD, NULL },
	{ "beeper.off", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "3", HU_TYPE_CMD, NULL },
	{ "beeper.enable", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "2", HU_TYPE_CMD, NULL },
	{ "beeper.disable", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "1", HU_TYPE_CMD, NULL },
	{ "beeper.mute", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "3", HU_TYPE_CMD, NULL },
	{ "beeper.mute", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "3", HU_TYPE_CMD, NULL },
	{ "beeper.mute", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "3", HU_TYPE_CMD, NULL },
	{ "ups.beeper.status", 0, 0, "UPS.PowerSummary.AudibleAlarmControl", NULL, "%s", 0, beeper_info },

	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.Charging", NULL, NULL, HU_FLAG_QUICK_POLL, charging_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.ACPresent", NULL, NULL, HU_FLAG_QUICK_POLL, online_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.Discharging", NULL, NULL, HU_FLAG_QUICK_POLL, discharging_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.BatteryPresent", NULL, NULL, 0, nobattery_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.BelowRemainingCapacityLimit", NULL, NULL, HU_FLAG_QUICK_POLL, lowbatt_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.ShutdownImminent", NULL, NULL, 0, shutdownimm_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.Overload", NULL, NULL, 0, overload_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.NeedReplacement", NULL, NULL, 0, replacebatt_info },
	{ "BOOL", 0, 0, "UPS.PowerSummary.PresentStatus.RemainingTimeLimitExpired", NULL, NULL, 0, timelimitexpired_info },

// Be very careful in case if change variable names. Check usbhid-ups::instcmd
	{ "outlet.1.load.off", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTOFF | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USBPORT  },
	{ "outlet.1.load.off.delay", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTOFF | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.1.load.on", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.1.load.on.delay", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.1.load.on.coldboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USBPORT | BF_OUTLETCOMMAND_COLDBOOTALLOWED },
	{ "outlet.1.load.reboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTREBOOT | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.1.load.shutdown", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTSHUTDOWN | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.1.load.canceloperation", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_CANCEL | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	
	{ "outlet.2.load.off", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTOFF | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.2.load.off.delay", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTOFF | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.2.load.on", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.2.load.on.delay", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.2.load.on.coldboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT | BF_OUTLETCOMMAND_COLDBOOTALLOWED },
	{ "outlet.2.load.reboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTREBOOT | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.2.load.shutdown", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTSHUTDOWN | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	{ "outlet.2.load.canceloperation", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_CANCEL | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },

	{ "load.off", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTOFF | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "load.off.delay", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTOFF | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "load.on", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "load.on.delay", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	{ "load.on.coldboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT | BF_OUTLETCOMMAND_COLDBOOTALLOWED },
	{ "load.reboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTREBOOT | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	{ "load.shutdown", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTSHUTDOWN | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },
	{ "load.canceloperation", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_CANCEL | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USEOFFDELAY | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_USBPORT },

	{ "outlet.1.delay.shutdown", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1034, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "outlet.1.delay.start", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1035, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "outlet.1.delay.stayoff", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1036, 2, DT_BINARYPOINT_S0, 0x0 },

	{ "outlet.2.delay.shutdown", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1039, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "outlet.2.delay.start", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f",  HU_FLAG_MODBUS, NULL, 1040, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "outlet.2.delay.stayoff", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1041, 2, DT_BINARYPOINT_S0, 0x0 },

	{ "ups.delay.shutdown", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1029, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "ups.delay.start", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1030, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "ups.delay.stayoff", ST_FLAG_RW, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL, 1031, 2, DT_BINARYPOINT_S0, 0x0 },

// Be very careful in case if change variable names. Check usbhid-ups::instcmd

	//{ "shutdown.stayoff", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1538, 2, DT_BITFIELD, BF_OUTLETCOMMAND_OUTPUTON | BF_OUTLETCOMMAND_USEONDELAY | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 | BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 | BF_OUTLETCOMMAND_USBPORT },
	//{ "shutdown.return", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1540, 1, DT_BITFIELD, BF_SIMPLESIGNALINGCOMMAND_REQUESTSHUTDOWN },
	{ "shutdown.reboot", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1540, 1, DT_BITFIELD, BF_SIMPLESIGNALINGCOMMAND_REQUESTSHUTDOWN },
	{ "shutdown.stop", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1540, 1, DT_BITFIELD, BF_SIMPLESIGNALINGCOMMAND_REQUESTSHUTDOWN },

	{ "test.battery.start.deep", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1542, 1, DT_BITFIELD, BF_RUNTIMECALIBRATIONCOMMAND_START },
	{ "test.battery.start.quick", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1541, 1, DT_BITFIELD, BF_RUNTIMECALIBRATIONCOMMAND_START },
	{ "test.battery.stop", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_TYPE_CMD, NULL, 1542, 1, DT_BITFIELD, BF_RUNTIMECALIBRATIONCOMMAND_ABORT },

	{ "ups.test.result", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS, apc_smt_MB_runtimecalibrationstatus_bw, 23, 1, DT_BITFIELD, 0x0 },
	{ "ups.test.result.deep", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS, apc_smt_MB_runtimecalibrationstatus_bw, 24, 1, DT_BITFIELD, 0x0 },
	{ "ups.test.result.quick", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS, apc_smt_MB_runtimecalibrationstatus_bw, 23, 1, DT_BITFIELD, 0x0 },
	{ "ups.test.result.runtimecalibration", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS, apc_smt_MB_runtimecalibrationstatus, 24, 1, DT_BITFIELD, 0x0 },
	{ "ups.test.result.battery", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS, apc_smt_MB_runtimecalibrationstatus, 23, 1, DT_BITFIELD, 0x0 },

	{ "outlet.1.status", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, apc_smt_MB_outletstatus_bw, 6, 2, DT_BITFIELD, 0x0 },
	{ "outlet.2.status", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, apc_smt_MB_outletstatus_bw, 9, 2, DT_BITFIELD, 0x0 },
	{ "outlet.1.status.native", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, apc_smt_MB_outletstatus, 6, 2, DT_BITFIELD, 0x0 },
	{ "outlet.2.status.native", 0, 0, "UPS.APCModbusRTUTx", NULL, "0", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, apc_smt_MB_outletstatus, 9, 2, DT_BITFIELD, 0x0 },

	// /*
	{ "output.voltage", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.2f", HU_FLAG_MODBUS, NULL,142, 1, DT_BINARYPOINT_U6, 0x0 },
	{ "output.current", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.2f", HU_FLAG_MODBUS, NULL,140, 1, DT_BINARYPOINT_U5, 0x0 },
	{ "output.frequency", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.2f", HU_FLAG_MODBUS, NULL,144, 1, DT_BINARYPOINT_U7, 0x0  },

	{ "ups.realpower", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL,136, 1, DT_BINARYPOINT_U8, 0x0 },

	{ "input.voltage", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.2f", HU_FLAG_MODBUS, NULL,151, 1, DT_BINARYPOINT_U6, 0x0 },
	
	{ "battery.temperature", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.2f", HU_FLAG_MODBUS, NULL,135, 1, DT_BINARYPOINT_S7, 0x0 },
	{ "battery.runtime", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS, NULL,128, 2, DT_BINARYPOINT_U0, 0x0  },

	{ "ups.timer.shutdown", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, NULL, 155, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "ups.timer.start", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, NULL, 156, 1, DT_BINARYPOINT_S0, 0x0 },
	{ "ups.timer.stayoff", 0, 0, "UPS.APCModbusRTUTx", NULL, "%.0f", HU_FLAG_MODBUS | HU_FLAG_QUICK_POLL, NULL, 157, 2, DT_BINARYPOINT_S0, 0x0 },

	/* end of structure. */
	{ NULL, 0, 0, NULL, NULL, NULL, 0, NULL }
};

#endif /* APC_HID_H */
