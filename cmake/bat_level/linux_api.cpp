#include "api.h"

#include <stdio.h>
#include <stdlib.h>

char get_battery_level() {
	const char* battery_path = "/sys/class/power_supply/BAT0/capacity";
	char buffer[4];

	FILE* battery_file = fopen( battery_path, "r");
	if( battery_file == NULL )
		return 0xff;
	fread( buffer, 1, sizeof(buffer), battery_file );
	fclose( battery_file );

	const char battery_level = (char) atoi( buffer );
	return battery_level;
}
