cmake_minimum_required(VERSION 3.0.0)

project(database VERSION 1.0.0 LANGUAGES C)

file(GLOB source src/*.c)
add_compile_options(-Wall -Wextra -Wshadow -O0 -g)
add_executable(data ${source})