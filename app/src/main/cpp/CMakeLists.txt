cmake_minimum_required(VERSION 3.18.1)

# Rename to crash-lib
add_library(crash-lib SHARED native-lib.cpp)

find_library(log-lib log)

target_link_libraries(crash-lib ${log-lib})