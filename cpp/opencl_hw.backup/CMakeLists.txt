cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(opencl_hello_world)


find_package(OpenCL REQUIRED)


set(EXECUTABLE_OUTPUT_PATH ./bin)
set(CMAKE_BUILD_DIRECTORY ./build)
file(GLOB source ./src/*.c ./src/*.cpp)

add_executable(main source)
target_link_libraries(main OpenCL::OpenCL)