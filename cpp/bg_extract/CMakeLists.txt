cmake_minimum_required(VERSION 3.9)
project(background_extract)

find_package(Threads REQUIRED)
find_package(OpenCV  REQUIRED)
find_package(X11     REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic -g -O1 -lX11 -L/usr/lib/X11 -lXext)

set(src src/main.cpp)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIR})
add_executable(main ${src})
target_link_libraries(main Threads::Threads)
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main ${X11_LIBRARIES})

target_compile_features(main PRIVATE cxx_std_20)