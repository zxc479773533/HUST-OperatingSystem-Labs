cmake_minimum_required(VERSION 3.9)

project(OS_labs)

# add zxcpyp lib
add_subdirectory(./zxcpyplib)

# build lab01
add_executable(pipe ./lab01-Pipe/pipe.c)
target_link_libraries(pipe zxcpyp_lib)

add_executable(fifo ./lab01-Pipe/fifo.c)
target_link_libraries(fifo zxcpyp_lib)

# build lab02
add_executable(sync ./lab02-Sync/sync.c)
target_link_libraries(sync zxcpyp_lib pthread)

# build lab03
add_executable(filecp ./lab03-FileCopy/filecp.c)
target_link_libraries(filecp zxcpyp_lib)