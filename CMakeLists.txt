cmake_minimum_required(VERSION 3.9)

project(OS_labs)

add_subdirectory(./zxcpyplib)

aux_source_directory(./lab01-Pipe dir_main)
add_executable(pipe ${dir_main})
target_link_libraries(pipe zxcpyp_lib)