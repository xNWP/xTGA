# CMakeLists.txt

cmake_minimum_required(VERSION 3.14)

project(xTGA	VERSION 100
				DESCRIPTION "A C/C++ library for opening/saving/manipulating TrueVision Targa (TGA) Images."
				LANGUAGES C CXX)

set(XTGA_VERSION ${PROJECT_VERSION})
set(XTGA_VERSION_ASCII "1.0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(xTGA)
add_subdirectory(tests)

# tests
add_test(TestDynamicLinkage test_dll)
add_test(TestStaticLinkage test_static)
add_test(TestTypeSize test_types)
add_test(TestLoading test_loading)
add_test(TestCLinkage test_c_linkage)
add_test(TestCLoading test_c_loading)

enable_testing()

# Standards
target_compile_features(xTGA PRIVATE
cxx_range_for
cxx_nullptr
cxx_lambdas
cxx_auto_type
cxx_rvalue_references
)

target_compile_features(xTGAs PRIVATE
cxx_range_for
cxx_nullptr
cxx_lambdas
cxx_auto_type
cxx_rvalue_references
)

# Packager
if(MSVC)
	add_custom_target(PACKAGE
	COMMENT "Packaging library for distribution..."
	COMMAND cd ${PROJECT_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} -E remove_directory build
	COMMAND mkdir build
	COMMAND mkdir packages
	COMMAND cd build
	COMMAND ${CMAKE_COMMAND} -G "Visual Studio 15 2017 Win64" ..
	COMMAND ${CMAKE_COMMAND} --build . --target xTGA --config Release
	COMMAND ${CMAKE_COMMAND} --build . --target xTGAs --config Release
	COMMAND ${CMAKE_COMMAND} --build . --target xTGA --config Debug
	COMMAND ${CMAKE_COMMAND} --build . --target xTGAs --config Debug
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA ${PROJECT_SOURCE_DIR}/build/include/xTGA
	COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_SOURCE_DIR}/build/include/xTGA/.gitignore
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/xTGA.dll ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/xTGA.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/xTGAs.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Debug/xTGA_d.dll ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Debug/xTGA_d.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Debug/xTGAs_d.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/LICENSE ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E tar cf ${PROJECT_SOURCE_DIR}/packages/xtga-${XTGA_VERSION_ASCII}-Win64.zip --format=zip -- xTGA.dll xTGA.lib xTGA_d.dll xTGA_d.lib xTGAs.lib xTGAs_d.lib include LICENSE
	COMMAND cd ..
	COMMAND ${CMAKE_COMMAND} -E remove_directory build

	COMMAND mkdir build
	COMMAND cd build
	COMMAND ${CMAKE_COMMAND} -G "Visual Studio 15 2017" ..
	COMMAND ${CMAKE_COMMAND} --build . --target xTGA --config Release
	COMMAND ${CMAKE_COMMAND} --build . --target xTGAs --config Release
	COMMAND ${CMAKE_COMMAND} --build . --target xTGA --config Debug
	COMMAND ${CMAKE_COMMAND} --build . --target xTGAs --config Debug
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA ${PROJECT_SOURCE_DIR}/build/include/xTGA
	COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_SOURCE_DIR}/build/include/xTGA/.gitignore
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/xTGA.dll ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/xTGA.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/xTGAs.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Debug/xTGA_d.dll ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Debug/xTGA_d.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Debug/xTGAs_d.lib ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/LICENSE ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E tar cf ${PROJECT_SOURCE_DIR}/packages/xtga-${XTGA_VERSION_ASCII}-Win32.zip --format=zip -- xTGA.dll xTGA.lib xTGA_d.dll xTGA_d.lib xTGAs.lib xTGAs_d.lib include LICENSE
	COMMAND cd ..
	COMMAND ${CMAKE_COMMAND} -E remove_directory build
)
elseif(UNIX)
	add_custom_target(PACKAGE
	COMMENT "Packaging library for distribution..."
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} chmod +x .travis/package_files_linux.sh
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} .travis/package_files_linux.sh ${XTGA_VERSION_ASCII}
)
endif()
