cmake_minimum_required(VERSION 3.9)
project(sched)

find_package(Threads REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic -g -O0)
set(src src/main.cpp)
add_executable(main ${src})
target_link_libraries(main Threads::Threads)
target_compile_features(main PRIVATE cxx_std_20)
