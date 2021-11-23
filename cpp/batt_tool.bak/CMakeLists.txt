cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_RPATH build)
set (CMAKE_C_STANDARD 99)

project(batt_tool)
add_executable(${PROJECT_NAME}/src main.c)
add_compile_options(-Wall -Wconversion -Wextra -Wpedantic -Werror -O0 -fno-exceptions)
#target_compile_features(intro PRIVATE cxx_lambda_init_captures)

#set_target_properties(main PROPERTIES)
RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}/.." )
