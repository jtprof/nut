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

#include <math.h>
#include "smtmodbus.h"

#define SMT_MESSAGES_DEBUG_LEVEL 4

info_lkp_t start_auto_info[] = {
	{ -1, "no", NULL },
	{ 1, "yes", NULL },
	{ 0, NULL, NULL }
};

// Code is generated automatically upon 'Modbus Implementation in APC Smart-UPS' (http://www.apc.com/salestools/MPAO-98KJ7F/MPAO-98KJ7F_R1_EN.pdf)

static const char *apc_smt_MB_upsstatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S00");
	if ( v & (uint64_t)BF_UPSSTATUS_STATEONLINE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOnline");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOnline");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_STATEONBATTERY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOnBattery");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOnBattery");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_STATEOUTPUTOFF ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOutputOff");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOutputOff");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_FAULT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Fault");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Fault");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_INPUTBAD ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InputBad");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InputBad");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_TEST ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Test");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Test");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_PENDINGOUTPUTON ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PendingOutputOn");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PendingOutputOn");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_PENDINGOUTPUTOFF ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PendingOutputOff");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PendingOutputOff");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_HIGHEFFICIENCY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "HighEfficiency");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "HighEfficiency");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_UPSSTATUS_INFORMATIONALALERT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InformationalAlert");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InformationalAlert");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsstatus_fun. S10 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_simplesignalingstatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingstatus_fun. S00");
	if ( v & (uint64_t)BF_SIMPLESIGNALINGSTATUS_POWERFAILURE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PowerFailure");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PowerFailure");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingstatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_SIMPLESIGNALINGSTATUS_SHUTDOWNIMMINENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ShutdownImminent");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ShutdownImminent");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingstatus_fun. S02 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_generalerror_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S00");
	if ( v & (uint64_t)BF_GENERALERROR_SITEWIRING ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SiteWiring");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SiteWiring");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_GENERALERROR_EEPROM ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "EEPROM");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "EEPROM");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_GENERALERROR_ADCONVERTER ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ADConverter");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ADConverter");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_GENERALERROR_LOGICPOWERSUPPLY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LogicPowerSupply");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LogicPowerSupply");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_GENERALERROR_INTERNALCOMMUNICATION ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InternalCommunication");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InternalCommunication");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_GENERALERROR_UIBUTTON ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "UIButton");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "UIButton");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_GENERALERROR_EPOACTIVE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "EPOActive");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "EPOActive");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_generalerror_fun. S07 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_powersystemerror_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S00");
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_OUTPUTOVERLOAD ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputOverload");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputOverload");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_OUTPUTSHORTCIRCUIT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputShortCircuit");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputShortCircuit");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_OUTPUTOVERVOLTAGE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputOvervoltage");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputOvervoltage");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_OVERTEMPERATURE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Overtemperature");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Overtemperature");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_BACKFEEDRELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "BackfeedRelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "BackfeedRelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_AVRRELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "AVRRelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "AVRRelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_PFCINPUTRELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PFCInputRelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PFCInputRelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_OUTPUTRELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputRelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputRelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_BYPASSRELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "BypassRelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "BypassRelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_PFC ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PFC");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PFC");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S10 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_DCBUSOVERVOLTAGE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "DCBusOvervoltage");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "DCBusOvervoltage");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S11 bp = %d", bp);
	if ( v & (uint64_t)BF_POWERSYSTEMERROR_INVERTER ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Inverter");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Inverter");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_powersystemerror_fun. S12 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_batterysystemerror_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S00");
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_DISCONNECTED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Disconnected");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Disconnected");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_OVERVOLTAGE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Overvoltage");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Overvoltage");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_NEEDSREPLACEMENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "NeedsReplacement");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "NeedsReplacement");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_OVERTEMPERATURECRITICAL ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OvertemperatureCritical");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OvertemperatureCritical");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_CHARGER ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Charger");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Charger");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_TEMPERATURESENSOR ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "TemperatureSensor");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "TemperatureSensor");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_BATTERYSYSTEMERROR_BUSSOFTSTART ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "BusSoftStart");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "BusSoftStart");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_batterysystemerror_fun. S07 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_replacebatteryteststatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S00");
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_PENDING ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Pending");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Pending");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_INPROGRESS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InProgress");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InProgress");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_PASSED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Passed");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Passed");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_FAILED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Failed");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Failed");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_REFUSED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Refused");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Refused");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_ABORTED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Aborted");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Aborted");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_PROTOCOL ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Protocol");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Protocol");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_LOCALUI ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LocalUI");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LocalUI");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_INTERNAL ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Internal");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Internal");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_INVALIDSTATE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InvalidState");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InvalidState");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S10 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_INTERNALFAULT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InternalFault");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InternalFault");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S11 bp = %d", bp);
	if ( v & (uint64_t)BF_REPLACEBATTERYTESTSTATUS_STATEOFCHARGENOTACCEPTABLE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOfChargeNotAcceptable");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOfChargeNotAcceptable");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_replacebatteryteststatus_fun. S12 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_runtimecalibrationstatus_fun_bw(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	// Speciat exception for Infinidat to backward campatibility. Start
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S00");
	if (v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_PENDING){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Test scheduled");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Test scheduled");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S01 bp = %d", bp);
	if (v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_INPROGRESS){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "In progress");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "In progress");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S02 bp = %d", bp);
	if (v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_PASSED){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Done and passed");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Done and passed");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S03 bp = %d", bp);
	if (v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_FAILED){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Done and error");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Done and error");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S04 bp = %d", bp);
	if (v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_REFUSED){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "No test initiated");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "No test initiated");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S05 bp = %d", bp);
	if (v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_ABORTED){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Aborted");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Aborted");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S06 bp = %d", bp);
	if (bp == 0) {
		snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Done and passed");
	}
	
	return ret;
	// Speciat exception for Infinidat to backward campatibility. End
}

static const char *apc_smt_MB_runtimecalibrationstatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S00");
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_PENDING ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Pending");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Pending");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_INPROGRESS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InProgress");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InProgress");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_PASSED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Passed");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Passed");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_FAILED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Failed");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Failed");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_REFUSED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Refused");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Refused");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_ABORTED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Aborted");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Aborted");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_PROTOCOL ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Protocol");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Protocol");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_LOCALUI ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LocalUI");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LocalUI");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_INTERNAL ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Internal");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Internal");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_INVALIDSTATE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InvalidState");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InvalidState");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S10 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_INTERNALFAULT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "InternalFault");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "InternalFault");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S11 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_STATEOFCHARGENOTACCEPTABLE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOfChargeNotAcceptable");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOfChargeNotAcceptable");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S12 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_LOADCHANGE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LoadChange");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LoadChange");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S13 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_ACINPUTNOTACCEPTABLE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ACInputNotAcceptable");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ACInputNotAcceptable");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S14 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_LOADTOOLOW ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LoadTooLow");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LoadTooLow");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S15 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONSTATUS_OVERCHARGEINPROGRESS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OverChargeInProgress");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OverChargeInProgress");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationstatus_fun. S16 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_lifetimestatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_lifetimestatus_fun. S00");
	if ( v & (uint64_t)BF_LIFETIMESTATUS_LIFETIMESTATUSOK ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LifeTimeStatusOK");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LifeTimeStatusOK");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_lifetimestatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_LIFETIMESTATUS_LIFETIMENEAREND ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LifeTimeNearEnd");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LifeTimeNearEnd");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_lifetimestatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_LIFETIMESTATUS_LIFETIMEEXCEEDED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LifeTimeExceeded");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LifeTimeExceeded");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_lifetimestatus_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_LIFETIMESTATUS_LIFETIMENEARENDACKNOWLEDGED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LifeTimeNearEndAcknowledged");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LifeTimeNearEndAcknowledged");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_lifetimestatus_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_LIFETIMESTATUS_LIFETIMEEXCEEDEDACKNOWLEDGED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LifeTimeExceededAcknowledged");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LifeTimeExceededAcknowledged");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_lifetimestatus_fun. S05 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_userinterfacestatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacestatus_fun. S00");
	if ( v & (uint64_t)BF_USERINTERFACESTATUS_CONTINUOUSTESTINPROGRESS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ContinuousTestInProgress");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ContinuousTestInProgress");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacestatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_USERINTERFACESTATUS_AUDIBLEALARMINPROGRESS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "AudibleAlarmInProgress");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "AudibleAlarmInProgress");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacestatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_USERINTERFACESTATUS_AUDIBLEALARMMUTED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "AudibleAlarmMuted");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "AudibleAlarmMuted");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacestatus_fun. S03 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_input_inputstatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S00");
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_ACCEPTABLE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Acceptable");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Acceptable");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_PENDINGACCEPTABLE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PendingAcceptable");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PendingAcceptable");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_VOLTAGETOOLOW ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "VoltageTooLow");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "VoltageTooLow");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_VOLTAGETOOHIGH ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "VoltageTooHigh");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "VoltageTooHigh");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_DISTORTED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Distorted");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Distorted");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_BOOST ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Boost");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Boost");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_TRIM ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Trim");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Trim");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_FREQUENCYTOOLOW ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "FrequencyTooLow");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "FrequencyTooLow");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_FREQUENCYTOOHIGH ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "FrequencyTooHigh");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "FrequencyTooHigh");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_INPUT_INPUTSTATUS_FREQANDPHASENOTLOCKED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "FreqAndPhaseNotLocked");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "FreqAndPhaseNotLocked");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_input_inputstatus_fun. S10 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_voltageacsetting_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_voltageacsetting_fun. S00");
	if ( v & (uint64_t)BF_VOLTAGEACSETTING_VAC120 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "VAC120");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "VAC120");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_voltageacsetting_fun. S01 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_acceptablefrequencysetting_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_acceptablefrequencysetting_fun. S00");
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_upscommand_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upscommand_fun. S00");
	if ( v & (uint64_t)BF_UPSCOMMAND_RESTOREFACTORYSETTINGS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "RestoreFactorySettings");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "RestoreFactorySettings");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upscommand_fun. S01 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_simplesignalingcommand_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingcommand_fun. S00");
	if ( v & (uint64_t)BF_SIMPLESIGNALINGCOMMAND_REQUESTSHUTDOWN ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "RequestShutdown");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "RequestShutdown");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingcommand_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_SIMPLESIGNALINGCOMMAND_REMOTEOFF ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "RemoteOff");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "RemoteOff");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingcommand_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_SIMPLESIGNALINGCOMMAND_REMOTEON ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "RemoteOn");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "RemoteOn");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_simplesignalingcommand_fun. S03 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_upsteststatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsteststatus_fun. S00");
	if ( v & (uint64_t)BF_UPSTESTSTATUS_START ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Start");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Start");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_upsteststatus_fun. S01 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_runtimecalibrationcommand_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationcommand_fun. S00");
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONCOMMAND_START ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Start");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Start");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationcommand_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_RUNTIMECALIBRATIONCOMMAND_ABORT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Abort");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Abort");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_runtimecalibrationcommand_fun. S02 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_userinterfacecommand_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacecommand_fun. S00");
	if ( v & (uint64_t)BF_USERINTERFACECOMMAND_SHORTTEST ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ShortTest");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ShortTest");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacecommand_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_USERINTERFACECOMMAND_CONTINUOUSTEST ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ContinuousTest");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ContinuousTest");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacecommand_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_USERINTERFACECOMMAND_MUTEALLACTIVEAUDIBLEALARMS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "MuteAllActiveAudibleAlarms");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "MuteAllActiveAudibleAlarms");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacecommand_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_USERINTERFACECOMMAND_CANCELMUTE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "CancelMute");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "CancelMute");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacecommand_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_USERINTERFACECOMMAND_ACKNOWLEDGEBATTERYALARMS ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "AcknowledgeBatteryAlarms");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "AcknowledgeBatteryAlarms");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_userinterfacecommand_fun. S05 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_outletstatus_fun_bw(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	// Speciat exception for Infinidat to backward campatibility. Start
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S00");
	if (v & (uint64_t)BF_OUTLETSTATUS_STATEON){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "on");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "on");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S01 bp = %d", bp);
	if (v & (uint64_t)BF_OUTLETSTATUS_STATEOFF)
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "off");
		}
		else {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "off");
			ff = true;
		}

	if (!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_outletstatus_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S00");
	if ( v & (uint64_t)BF_OUTLETSTATUS_STATEON ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOn");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOn");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_STATEOFF ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "StateOff");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "StateOff");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_PROCESSREBOOT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ProcessReboot");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ProcessReboot");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_PROCESSSHUTDOWN ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ProcessShutdown");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ProcessShutdown");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_PROCESSSLEEP ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ProcessSleep");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ProcessSleep");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_PENDINGOFFDELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PendingOffDelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PendingOffDelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_PENDINGONACPRESENCE ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PendingOnACPresence");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PendingOnACPresence");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_PENDINGONMINRUNTIME ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "PendingOnMinRuntime");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "PendingOnMinRuntime");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_MEMBERGROUPPROCESS1 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "MemberGroupProcess1");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "MemberGroupProcess1");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_MEMBERGROUPPROCESS2 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "MemberGroupProcess2");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "MemberGroupProcess2");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S10 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETSTATUS_LOWRUNTIME ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LowRuntime");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LowRuntime");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletstatus_fun. S11 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_outletcommand_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S00");
	if ( v & (uint64_t)BF_OUTLETCOMMAND_CANCEL ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "Cancel");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "Cancel");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_OUTPUTON ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputOn");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputOn");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_OUTPUTOFF ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputOff");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputOff");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_OUTPUTSHUTDOWN ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputShutdown");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputShutdown");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_OUTPUTREBOOT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "OutputReboot");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "OutputReboot");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S05 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_COLDBOOTALLOWED ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "ColdBootAllowed");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "ColdBootAllowed");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S06 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_USEONDELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "UseOnDelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "UseOnDelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S07 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_USEOFFDELAY ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "UseOffDelay");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "UseOffDelay");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S08 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_UNSWITCHEDOUTLETGROUP ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "UnswitchedOutletGroup");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "UnswitchedOutletGroup");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S09 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP0 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SwitchedOutletGroup0");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SwitchedOutletGroup0");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S10 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP1 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SwitchedOutletGroup1");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SwitchedOutletGroup1");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S11 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_SWITCHEDOUTLETGROUP2 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SwitchedOutletGroup2");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SwitchedOutletGroup2");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S12 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_USBPORT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "USBPort");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "USBPort");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S13 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_LOCALUSER ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "LocalUser");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "LocalUser");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S14 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_RJ45PORT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "RJ45Port");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "RJ45Port");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S15 bp = %d", bp);
	if ( v & (uint64_t)BF_OUTLETCOMMAND_SMARTSLOT1 ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SmartSlot1");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SmartSlot1");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_outletcommand_fun. S16 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};

static const char *apc_smt_MB_i_sogrelayconfigsetting_fun(double value) {
	uint64_t v = (uint64_t)value;
	static char ret[255] = {0};
	uint8_t bp = 0;
	bool ff = false;

	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_i_sogrelayconfigsetting_fun. S00");
	if ( v & (uint64_t)BF_I_SOGRELAYCONFIGSETTING_MOGPRESENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "MOGPresent");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "MOGPresent");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_i_sogrelayconfigsetting_fun. S01 bp = %d", bp);
	if ( v & (uint64_t)BF_I_SOGRELAYCONFIGSETTING_SOG0PRESENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SOG0Present");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SOG0Present");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_i_sogrelayconfigsetting_fun. S02 bp = %d", bp);
	if ( v & (uint64_t)BF_I_SOGRELAYCONFIGSETTING_SOG1PRESENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SOG1Present");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SOG1Present");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_i_sogrelayconfigsetting_fun. S03 bp = %d", bp);
	if ( v & (uint64_t)BF_I_SOGRELAYCONFIGSETTING_SOG2PRESENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SOG2Present");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SOG2Present");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_i_sogrelayconfigsetting_fun. S04 bp = %d", bp);
	if ( v & (uint64_t)BF_I_SOGRELAYCONFIGSETTING_SOG3PRESENT ){
		if (ff) {
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "-%s", "SOG3Present");
		}else{
			bp = snprintf(&ret[bp], sizeof(ret) - bp, "%s", "SOG3Present");
			ff = true;
		}

	}
	upsdebugx(SMT_MESSAGES_DEBUG_LEVEL, "smtmodbus::apc_smt_MB_i_sogrelayconfigsetting_fun. S05 bp = %d", bp);
	if(!ff) snprintf(&ret[bp], sizeof(ret) - bp, "unknown");
	return ret;
};


// Code is generated automatically upon 'Modbus Implementation in APC Smart-UPS' (http://www.apc.com/salestools/MPAO-98KJ7F/MPAO-98KJ7F_R1_EN.pdf)

info_lkp_t apc_smt_MB_upsstatus[] = {
	{ 0, NULL, apc_smt_MB_upsstatus_fun }
};
info_lkp_t apc_smt_MB_simplesignalingstatus[] = {
	{ 0, NULL, apc_smt_MB_simplesignalingstatus_fun }
};
info_lkp_t apc_smt_MB_generalerror[] = {
	{ 0, NULL, apc_smt_MB_generalerror_fun }
};
info_lkp_t apc_smt_MB_powersystemerror[] = {
	{ 0, NULL, apc_smt_MB_powersystemerror_fun }
};
info_lkp_t apc_smt_MB_batterysystemerror[] = {
	{ 0, NULL, apc_smt_MB_batterysystemerror_fun }
};
info_lkp_t apc_smt_MB_replacebatteryteststatus[] = {
	{ 0, NULL, apc_smt_MB_replacebatteryteststatus_fun }
};
info_lkp_t apc_smt_MB_runtimecalibrationstatus[] = {
	{ 0, NULL, apc_smt_MB_runtimecalibrationstatus_fun }
};
info_lkp_t apc_smt_MB_runtimecalibrationstatus_bw[] = {
	{ 0, NULL, apc_smt_MB_runtimecalibrationstatus_fun_bw }
};
info_lkp_t apc_smt_MB_lifetimestatus[] = {
	{ 0, NULL, apc_smt_MB_lifetimestatus_fun }
};
info_lkp_t apc_smt_MB_userinterfacestatus[] = {
	{ 0, NULL, apc_smt_MB_userinterfacestatus_fun }
};
info_lkp_t apc_smt_MB_input_inputstatus[] = {
	{ 0, NULL, apc_smt_MB_input_inputstatus_fun }
};
info_lkp_t apc_smt_MB_voltageacsetting[] = {
	{ 0, NULL, apc_smt_MB_voltageacsetting_fun }
};
info_lkp_t apc_smt_MB_acceptablefrequencysetting[] = {
	{ 0, NULL, apc_smt_MB_acceptablefrequencysetting_fun }
};
info_lkp_t apc_smt_MB_upscommand[] = {
	{ 0, NULL, apc_smt_MB_upscommand_fun }
};
info_lkp_t apc_smt_MB_simplesignalingcommand[] = {
	{ 0, NULL, apc_smt_MB_simplesignalingcommand_fun }
};
info_lkp_t apc_smt_MB_upsteststatus[] = {
	{ 0, NULL, apc_smt_MB_upsteststatus_fun }
};
info_lkp_t apc_smt_MB_runtimecalibrationcommand[] = {
	{ 0, NULL, apc_smt_MB_runtimecalibrationcommand_fun }
};
info_lkp_t apc_smt_MB_userinterfacecommand[] = {
	{ 0, NULL, apc_smt_MB_userinterfacecommand_fun }
};
info_lkp_t apc_smt_MB_outletstatus[] = {
	{ 0, NULL, apc_smt_MB_outletstatus_fun }
};
info_lkp_t apc_smt_MB_outletstatus_bw[] = {
	{ 0, NULL, apc_smt_MB_outletstatus_fun_bw }
};
info_lkp_t apc_smt_MB_outletcommand[] = {
	{ 0, NULL, apc_smt_MB_outletcommand_fun }
};
info_lkp_t apc_smt_MB_i_sogrelayconfigsetting[] = {
	{ 0, NULL, apc_smt_MB_i_sogrelayconfigsetting_fun }
};
