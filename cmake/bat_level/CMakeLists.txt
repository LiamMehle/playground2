cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_RPATH build)

project(batt_level)

IF (MINGW OR WIN32)
	add_executable(${PROJECT_NAME} main.cpp win_api.cpp)
ELSE()
	add_executable(${PROJECT_NAME} main.cpp linux_api.cpp)
ENDIF()

target_compile_features(${PROJECT_NAME} PUBLIC
	cxx_std_11
	cxx_explicit_conversions
	cxx_noexcept
)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Ofast -march=native -fnoexcept)
#target_compile_features(intro PRIVATE cxx_lambda_init_captures)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}/.." )
