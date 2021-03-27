cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_RPATH build)
set(CMAKE_C_STANDARD  99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ./bin/)
file(GLOB sources
     "src/*.c"
     "src/*.cpp"
)
project(batt_tool)
add_executable(batt_tool ${sources})
add_compile_options(-Wall -Wconversion -Wextra -Wpedantic -Werror -Ofast -fno-exceptions)
#target_compile_features(intro PRIVATE cxx_lambda_init_captures)

#set_target_properties(main PROPERTIES
#RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}/.." )
