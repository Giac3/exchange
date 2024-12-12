cmake_minimum_required(VERSION 3.7)
project(exchange)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

FILE(GLOB_RECURSE MyCSources src/*.c)
add_executable(exchange ${MyCSources})

FILE(GLOB_RECURSE MyCTestSources tests/*.c)
add_executable(test_exchange ${MyCTestSources})

target_include_directories(exchange PRIVATE include)