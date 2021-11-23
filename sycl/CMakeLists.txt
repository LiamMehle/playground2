cmake_minimum_required(VERSION 3.4.6)
project(matrixMultiply)

set(SRCS "src/main.cpp"
         "src/program.cpp")

set(SYCL_FILES
    "src/main.cpp")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

#set(ComputeCpp_DIR "/opt/ComputeCpp-CE-2.3.0-x86_64-linux-gnu/")


if (NOT ComputeCpp_DIR)
    message(FATAL_ERROR
    "SYCL implementation root not provided, please specify "
    "the path to the root of the chosen SYCL implementation using "
    "ComputeCpp_DIR=<path/to/install/root>.")
endif()


#set(ComputeCpp_INCLUDE_DIRS "${ComputeCpp_DIR}/include")
set(CMAKE_MODULE_PATH "/home/wolf/git/computecpp-sdk/cmake/Modules")
find_package(ComputeCpp REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS})
message("{PROJECT_NAME} = ${PROJECT_NAME}")
message("{ComputeCpp_INCLUDE_DIRS} = ${ComputeCpp_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME} PRIVATE
                          ${ComputeCpp_INCLUDE_DIRS}
                          "${CMAKE_SOURCE_DIR}/src"
                          )
target_link_libraries(${PROJECT_NAME} PRIVATE 
                      ${COMPUTECPP_RUNTIME_LIBRARY}
                      ${COMPUTECPP_RUNTIME_LIBRARY_DEBUG}
                      ${COMPUTECPP_RUNTIME_LIBRARY}
                      )
add_sycl_to_target(TARGET ${PROJECT_NAME}
                   SOURCES "${CMAKE_SOURCE_DIR}/${SYCL_FILES}")
                    