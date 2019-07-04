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
	list(APPEND packageList
	xTGA.dll
	xTGA_d.dll
	xTGA.lib
	xTGA_d.lib
	xTGAs.lib
	xTGAs_d.lib
	LICENSE
	${HEADERS})

	if(CMAKE_CL_64)
		set(archiveName xtga-${XTGA_VERSION_ASCII}-Win64)
	else()
		set(archiveName xtga-${XTGA_VERSION_ASCII}-Win32)
	endif()

	add_custom_target(PACKAGE
	COMMENT "Packaging library for distribution..."
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} cmake --build . --target xTGA --config Release
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} cmake --build . --target xTGAs --config Release
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA ${PROJECT_BINARY_DIR}/temp/include/xTGA
	COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/temp/include/xTGA/.gitignore
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/Release/xTGA.dll ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/Release/xTGA.lib ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/Release/xTGAs.lib ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/LICENSE ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR}/temp tar cf ${PROJECT_BINARY_DIR}/${archiveName}.zip --format=zip -- ${packageList}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/temp
)
elseif(UNIX)
	list(APPEND packageList
	libxTGA.so
	libxTGA_d.so
	libxTGAs.a
	libxTGAs_d.a
	LICENSE
	${HEADERS})

	if(CMAKE_CXX_FLAGS MATCHES "-m32")
		set(archiveName xtga-${XTGA_VERSION_ASCII}-linux-x86)
	else()
		set(archiveName xtga-${XTGA_VERSION_ASCII}-linux-x64)
	endif()

	add_custom_target(PACKAGE
	COMMENT "Packaging library for distribution..."
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} cmake --build . --target xTGA
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} cmake --build . --target xTGAs
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA ${PROJECT_BINARY_DIR}/temp/include/xTGA
	COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/temp/include/xTGA/.gitignore
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libxTGA.so ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libxTGAs.a ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/LICENSE ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR}/temp tar czf ${PROJECT_BINARY_DIR}/${archiveName}.tar.gz -- ${packageList}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/temp
)
endif()
