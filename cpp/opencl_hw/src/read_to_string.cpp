#include "read_to_string.hpp"
#include <fstream>

std::string read_to_string( const std::string& path ) {
	std::ifstream stream( path, std::ifstream::in );
	stream.seekg( 0, stream.end );
	const long len = stream.tellg();
	stream.seekg( 0, stream.beg );
	std::string ret( len, ' ' );
	stream.read( &ret[0], len );
	return ret;
}