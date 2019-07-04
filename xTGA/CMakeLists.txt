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
target_compile_definitions(xTGA PRIVATE XTGA_EXPORTS)
target_compile_definitions(xTGAs PUBLIC XTGA_STATIC)

set_property(TARGET xTGA xTGAs PROPERTY DEBUG_POSTFIX _d)

if(UNIX)
	find_package(Threads)
	target_link_libraries(xTGA ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(xTGAs ${CMAKE_THREAD_LIBS_INIT})
endif()