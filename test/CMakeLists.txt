cmake_minimum_required(VERSION 3.27)
project(koisock_test)


set(KOISOCK_DEBUG koisock_debug)

add_executable(${KOISOCK_DEBUG}
    debug.cpp
)

target_link_libraries(${KOISOCK_DEBUG} PRIVATE
    koisock
)