cmake_minimum_required(VERSION 4.3)
project(static_allocator C)

set(CMAKE_C_STANDARD 23)

add_library(my_alloc SHARED
        lib/my_alloc/my_alloc.c
        lib/my_alloc/my_alloc.h
)

target_include_directories(my_alloc PUBLIC my_alloc)

add_executable(static_allocator main.c)

target_link_libraries(static_allocator PRIVATE my_alloc)
