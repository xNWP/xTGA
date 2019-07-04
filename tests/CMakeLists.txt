# Configs
list(APPEND CFGHEADERS
cmake_file.h
)

set(DATASETDIR ${PROJECT_SOURCE_DIR}/tests/data)

file(REMOVE .gitignore)
file(APPEND .gitignore "# CMake Generated Headers\n\n")

foreach(x IN LISTS CFGHEADERS)
	set(y)
	string(REGEX REPLACE "cmake_*" "" y ${x})
	file(APPEND .gitignore ${y}\n)
	configure_file(${x} ${PROJECT_SOURCE_DIR}/tests/${y})
endforeach()

set(interface ${PROJECT_SOURCE_DIR}/xTGA/include)
set(common ${PROJECT_SOURCE_DIR}/tests)

add_executable(test_dll main.cpp)
target_link_libraries(test_dll xTGA)
target_include_directories(test_dll PUBLIC ${interface})

add_executable(test_static main.cpp)
target_link_libraries(test_static xTGAs)
target_include_directories(test_static PUBLIC ${interface})

add_executable(test_types types.cpp)
target_include_directories(test_types PUBLIC ${interface})

add_executable(test_loading loading.cpp assert_equal.h library_error.h cmake_file.h)
target_link_libraries(test_loading xTGA)
target_include_directories(test_loading PUBLIC ${interface} ${common})

add_executable(test_c_linkage c_linkage.c)
target_link_libraries(test_c_linkage xTGA)
target_include_directories(test_c_linkage PUBLIC ${interface})

add_executable(test_c_loading c_loading.c assert_equal.h library_error.h cmake_file.h)
target_link_libraries(test_c_loading xTGA)
target_include_directories(test_c_loading PUBLIC ${interface} ${common})