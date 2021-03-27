#include "api.h"

#include <windows.h>

char get_battery_level() {
	SYSTEM_POWER_STATUS status;
	GetSystemPowerStatus( &status );
	return status.BatteryLifePercent;
}
