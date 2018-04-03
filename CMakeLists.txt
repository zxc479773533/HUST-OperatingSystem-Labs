cmake_minimum_required(VERSION 3.9)

project(OS_labs)

add_subdirectory(./zxcpyplib)

add_executable(pipe ./lab01-Pipe/pipe.c)
target_link_libraries(pipe zxcpyp_lib)

add_executable(fifo ./lab01-Pipe/fifo.c)
target_link_libraries(fifo zxcpyp_lib)