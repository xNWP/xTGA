add_executable(test_dll main.cpp)
add_executable(test_static main.cpp)
target_link_libraries(test_dll xTGA)
target_link_libraries(test_static xTGAs)

target_include_directories(test_dll PUBLIC ${PROJECT_SOURCE_DIR}/xTGA/include)
target_include_directories(test_static PUBLIC ${PROJECT_SOURCE_DIR}/xTGA/include)
target_compile_definitions(test_static PUBLIC XTGA_STATIC)