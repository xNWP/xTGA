# CMakeLists.txt

cmake_minimum_required(VERSION 3.8)

project(xTGA	VERSION 100
				DESCRIPTION "A C/C++ library for opening/saving/manipulating TrueVision Targa (TGA) Images."
				LANGUAGES CXX)
				
set(XTGA_VERSION ${PROJECT_VERSION})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(xTGA)
add_subdirectory(tests)

enable_testing()
add_test(TestDynamicLinkage test_dll)
add_test(TestStaticLinkage test_static)

# Standards
target_compile_features(xTGA PRIVATE
cxx_range_for
cxx_nullptr
cxx_lambdas
cxx_auto_type
cxx_rvalue_references
)