cmake_minimum_required(VERSION 3.5)

project(dedupe)

set(CMAKE_BUILD_RPATH build)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ./bin/)
file(GLOB sources
     #"src/*.c"
     "src/*.cpp"
)
add_compile_options(-O3)
add_executable(dedupe ${sources})
target_compile_features(dedupe PRIVATE
	cxx_lambda_init_captures
	cxx_decltype
	cxx_lambdas
	cxx_range_for
	cxx_trailing_return_types
)

find_package(Threads)
target_link_libraries(dedupe ${CMAKE_THREAD_LIBS_INIT})