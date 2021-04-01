#include "create_shader.hpp"
#include <iostream>

static unsigned int compile_source( unsigned int type, const std::string& source ) {
	unsigned id = glCreateShader( type );
	const char* const src = source.c_str();
	glShaderSource( id, 1, &src, nullptr );
	glCompileShader( id );

	int result;
	glGetShaderiv( id, GL_COMPILE_STATUS, &result );
	if( result == GL_FALSE ) {
		int len;
		glGetShaderiv( id, GL_INFO_LOG_LENGTH, &len );
		char* message = new char[len];
		glGetShaderInfoLog( id, len, &len, message );
		if( type == GL_VERTEX_SHADER )
			std::cout << "[ERROR] while compiling vertex shader\n";
		if( type == GL_FRAGMENT_SHADER )
			std::cout << "[ERROR] while compiling fragment shader\n";
		std::cout << "[ERROR] " << message << '\n';
		delete message;
		return 0;	
	}

	return id;
}

unsigned int create_shader(const std::string& vertex_shader,
                         const std::string& fragment_shader ) {
	unsigned int program = glCreateProgram(),
	             vs = compile_source( GL_VERTEX_SHADER, vertex_shader ),
	             fs = compile_source( GL_FRAGMENT_SHADER, fragment_shader );
	
	glAttachShader(    program, vs );
	glAttachShader(    program, fs );
	glLinkProgram(     program );
	glValidateProgram( program );

	glDeleteShader( vs );
	glDeleteShader( fs );
	return program;
}
