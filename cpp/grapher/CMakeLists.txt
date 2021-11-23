cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(grapher)




set(EXECUTABLE_OUTPUT_PATH ./bin)
set(CMAKE_BUILD_DIRECTORY ./build)

# MSVC unfriendly
add_compile_options(-Ofast -Wall -Wextra -Wshadow -g -march=skylake)

file(GLOB source ./src/*.c ./src/*.cpp)
add_executable(main ${source})

target_compile_features(main PUBLIC
	c_std_11
)


#find_package(OpenCL REQUIRED)
#target_link_libraries(main OpenCL::OpenCL)
