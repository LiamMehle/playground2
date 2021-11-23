cmake_minimum_required(VERSION 3.0)
project(gui_hw)

file(GLOB sources "src/*.c"
                  "src/*.cpp")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
find_package(Boost   REQUIRED)
find_package(Threads REQUIRED)

include_directories(${THREADS_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})

link_directories   (${THREADS_LIBRARY_DIRS})
link_directories   (${BOOST_LIBRARY_DIRS})
link_directories   (${GTK3_LIBRARY_DIRS})

add_executable(gui_hw ${sources})

target_link_libraries(gui_hw ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gui_hw ${GTK3_LIBRARIES})
target_link_libraries(gui_hw ${BOOST_LIBRARIES})
