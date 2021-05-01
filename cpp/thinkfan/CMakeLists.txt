cmake_minimum_required(VERSION 3.0.0)

project(thinkfanc)


set(EXECUTABLE_OUTPUT_PATH ./bin)
set(CMAKE_BUILD_DIRECTORY ./build)
set(CMAKE_THREAD_PREFER_PTHREAD)
set(THREADS_PREFER_PTHREAD_FLAG)

# MSVC unfriendly
add_compile_options(
	-Ofast -Wall -Wextra -Wshadow -Ofast -march=skylake -lstdc++fs -g
	-fsanitize=address
	-fopenmp
)
add_link_options(
	-lasan
)


file(GLOB source ./src/*.c ./src/*.cpp)
add_executable(${PROJECT_NAME} ${source})

target_compile_features(${PROJECT_NAME} PUBLIC
	cxx_std_17
	c_std_11
)


#find_package(OpenCL REQUIRED)
#target_link_libraries(main OpenCL::OpenCL)
