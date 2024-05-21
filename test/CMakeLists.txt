cmake_minimum_required(VERSION 3.27)
project(KoiNetTest)


set(KOISOCK_DEBUG KoiNetDebug)

add_executable(${KOISOCK_DEBUG}
    debug.cpp
)

target_link_libraries(${KOISOCK_DEBUG} PRIVATE
    KoiNet
)