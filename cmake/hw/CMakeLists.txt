cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_RPATH build)

project(hw)
add_executable(${PROJECT_NAME} main.c)
add_compile_options(-Wall -Wextra -Wpedantic)
#target_compile_features(intro PRIVATE cxx_lambda_init_captures)

set_target_properties(hw PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}/.." )
