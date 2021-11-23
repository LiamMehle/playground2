#include <stdio.h>

int set_level( const char* const cmd ) {
FILE* const dev = fopen("/proc/acpi/ibm/fan", "w");
	
	// check for errors
	const int pointer_valid = dev != NULL;

	// no error detected
	if( pointer_valid ) {
		const int ret = fprintf( dev, "level %s", cmd );
		fclose( dev );
		return ret;
	}
	return -1;
}