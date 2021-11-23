#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

unsigned int create_shader(const std::string& vertex_shader,
                         const std::string& fragment_shader );
