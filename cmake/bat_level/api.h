#pragma once
#define BATT_UNKNOWN 0xff
#define BATT_MAX      100
#define BATT_MIN        0
#define batt_is_valid(x) ( BATT_MIN <= x && x <= BATT_MAX )
#define batt_is_known(x) ( x != BATT_UNKNOWN )

char get_battery_level();
