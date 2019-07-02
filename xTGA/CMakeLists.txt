# xTGA Library

# Configs
list(APPEND CFGHEADERS
cmakeheaders/cmake_api.h
)

file(REMOVE include/xTGA/.gitignore)
file(APPEND include/xTGA/.gitignore "# CMake Generated Headers\n\n")

foreach(x IN LISTS CFGHEADERS)
	set(y)
	string(REGEX REPLACE "cmakeheaders/cmake_*" "" y ${x})
	file(APPEND include/xTGA/.gitignore ${y}\n)
	list(APPEND HEADERS include/xTGA/${y})
	configure_file(${x} ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA/${y})
endforeach()

add_compile_definitions(XTGA_EXPORTS)

set(SOURCES
src/codecs.h
src/codecs.cpp
src/error_macro.h
src/marray.cpp
src/pixelformats.cpp
src/tga_file.cpp
src/xTGA_C.cpp
)

list(APPEND HEADERS
include/xTGA/error.h
include/xTGA/flags.h
include/xTGA/marray.h
include/xTGA/pixelformats.h
include/xTGA/structures.h
include/xTGA/tga_file.h
include/xTGA/types.h
include/xTGA/xTGA.h
include/xTGA/xTGA_C.h
)

# Binary
add_library(xTGA SHARED ${SOURCES} ${HEADERS} ${CFGHEADERS})
add_library(xTGAs STATIC ${SOURCES} ${HEADERS} ${CFGHEADERS})
target_include_directories(xTGA PUBLIC include)
target_include_directories(xTGAs PUBLIC include)

if(UNIX)
	find_package(Threads)
	target_link_libraries(xTGA ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(xTGAs ${CMAKE_THREAD_LIBS_INIT})
endif()

# Packager
if(MSVC)
	list(APPEND packageList
	xTGA.dll
	xTGA.lib
	xTGAs.lib
	LICENSE
	${HEADERS})

	if(CMAKE_CL_64)
		set(archiveName xtga-${XTGA_VERSION_ASCII}-Win64)
	else()
		set(archiveName xtga-${XTGA_VERSION_ASCII}-Win32)
	endif()

	add_custom_target(PACKAGE
	COMMENT Packaging library for distribution...
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
	libxTGAs.a
	LICENSE
	${HEADERS})

	add_custom_target(PACKAGE
	COMMENT Packaging library for distribution...
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} cmake --build . --target xTGA
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} cmake --build . --target xTGAs
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA ${PROJECT_BINARY_DIR}/temp/include/xTGA
	COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/temp/include/xTGA/.gitignore
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libxTGA.so ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libxTGAs.a ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/LICENSE ${PROJECT_BINARY_DIR}/temp
	COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR}/temp tar czf ${PROJECT_BINARY_DIR}/${archiveName}.tar.gz --format=gnutar -- ${packageList}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/temp
)
endif()