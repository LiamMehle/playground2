#define GLEW_STATIC

#include "create_shader.hpp"
#include "read_to_string.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


#include <thread>
#include <chrono>
#include <string>
#include <fstream>

#define IOTA __COUNTER__

#define vertex_shader_path   "src/basic.vs"
#define fragment_shader_path "src/basic.fs"



using namespace std::chrono_literals;

enum screen_state {
	clean = 0
};


int terminate = 0;
int screen_state;
float vertecies[] = {
		-.5f, -.5f,
		 .0f,  .5f,
		 .5f, -.5f
};

[[maybe_unused]]
static void vertex_modify() {
	while( !terminate ) {
		std::cout << "cycle\n";
		std::this_thread::sleep_for(1s);
		vertecies[2] = .5;
		screen_state++;
		std::this_thread::sleep_for(1s);
		vertecies[2] = -.5;
		screen_state++;
	}
}

int main() {

	constexpr unsigned int width    = 640;
	constexpr unsigned int height   = 400;
	constexpr char         title[]  = "hello triangle";

	if( !glfwInit() )
		return -IOTA;

	auto* window = glfwCreateWindow( width, height, title, NULL, NULL );
	if( !window ) {
		glfwTerminate();
		return -IOTA;
	}
	
	glfwMakeContextCurrent( window );

	std::cout << "hi\n";
	std::cout << "vendor   = " << (char*)glGetString( GL_VENDOR   ) << '\n'<
	          << "renderer = " << (char*)glGetString( GL_RENDERER ) << '\n'<
	          << "version  = " << (char*)glGetString( GL_VERSION  ) << '\n'<
	          << '\n';

	if( glewInit() != GLEW_OK )
		std::cout << "glew failed to initialize!\n";


	constexpr char dimensions = 2;
	
	constexpr unsigned int vertex_count = sizeof(vertecies)/(2*sizeof(vertecies[0]));

	unsigned int gl_buffer_id;
	glGenBuffers( 1, &gl_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, gl_buffer_id );	
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertecies),
	              vertecies, GL_DYNAMIC_DRAW );
	glVertexAttribPointer( 0, dimensions, GL_FLOAT, GL_FALSE, dimensions*sizeof(vertecies[0]), 0 );
	glEnableVertexAttribArray( 0 );


	const std::string vertex_shader   = read_to_string( vertex_shader_path );
	const std::string fragment_shader = read_to_string( fragment_shader_path );


	unsigned int shader = create_shader( vertex_shader, fragment_shader );
	glUseProgram( shader );

	//std::thread compute( vertex_modify );
	while( !glfwWindowShouldClose(window) ) {
		//if( screen_state != 0 ) {
			//std::cout << "draw call\n";
			glClear( GL_COLOR_BUFFER_BIT );
			//std::this_thread::sleep_for(500ms);
			glDrawArrays(GL_TRIANGLES, 0, vertex_count );
			screen_state = 0;
			glfwSwapBuffers( window );
		//}

		glfwPollEvents();
	}
	//terminate = 1;
	glDeleteProgram( shader );
	glfwTerminate();

	//compute.join();

	return 0;
}