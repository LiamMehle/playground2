cmake_minimum_required(VERSION 3.17)
project(vulkan_hw)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB sources "src/*.c" "src/*.cpp")
add_executable(main ${sources})

target_include_directories(main PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(main PRIVATE      ${Vulkan_LIBRARY})
target_link_libraries(main PRIVATE glfw)
target_compile_features(main PRIVATE
cxx_std_20
cxx_constexpr)