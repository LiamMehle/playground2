cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_RPATH build)

project(sort)
file(GLOB source ./src/*.c ./src/*.cpp)
add_executable(sort ${source})


target_compile_features(${PROJECT_NAME} PUBLIC
	c_std_99
)
