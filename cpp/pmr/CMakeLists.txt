cmake_minimum_required(VERSION 3.9)
project(pmr)

find_package(Threads REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic -O1)
set(src src/main.cpp)
add_executable(main ${src})
target_link_libraries(main Threads::Threads)
target_compile_features(main PRIVATE cxx_std_20)
