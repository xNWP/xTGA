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
	configure_file(${x} ${PROJECT_SOURCE_DIR}/xTGA/include/xTGA/${y})
endforeach()

add_compile_definitions(XTGA_EXPORTS)

set(SOURCES
src/codecs.h
src/codecs.cpp
src/marray.cpp
src/pixelformats.cpp
src/tga_file.cpp
)

set(HEADERS
include/xTGA/error.h
include/xTGA/flags.h
include/xTGA/marray.h
include/xTGA/pixelformats.h
include/xTGA/structures.h
include/xTGA/tga_file.h
include/xTGA/types.h
include/xTGA/xTGA.h
)

# Binary
add_library(xTGA SHARED ${SOURCES} ${HEADERS} ${CFGHEADERS})
add_library(xTGAs STATIC ${SOURCES} ${HEADERS} ${CFGHEADERS})
target_include_directories(xTGA PUBLIC include)
target_include_directories(xTGAs PUBLIC include)
target_link_libraries(xTGA pthread)
target_link_libraries(xTGAs pthread)

