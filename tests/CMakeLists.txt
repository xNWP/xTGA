set(interface ${PROJECT_SOURCE_DIR}/xTGA/include)

add_executable(test_dll main.cpp)
target_link_libraries(test_dll xTGA)
target_include_directories(test_dll PUBLIC ${interface})

add_executable(test_static main.cpp)
target_link_libraries(test_static xTGAs)
target_include_directories(test_static PUBLIC ${interface})
target_compile_definitions(test_static PUBLIC XTGA_STATIC)

add_executable(test_types types.cpp)
target_link_libraries(test_types xTGA)
target_include_directories(test_types PUBLIC ${interface})