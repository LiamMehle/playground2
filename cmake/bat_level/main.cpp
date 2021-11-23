#include <stdio.h>

#include "api.h"

int main() {
	const char batt_level = get_battery_level();
	printf("battery: %u\n", (int)batt_level&0xff );
	return 0;
}
