cmake_minimum_required(VERSION 3.27)
project(koisock)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")


set(LIBS "")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LIBS
        ws2_32
        iphlpapi
    )
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

else ()
    message(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} is not supported!")
endif ()

message(STATUS "LIBS: ${LIBS}")

add_library(${PROJECT_NAME} STATIC
    source/socket_peer.cpp
    include/network/socket_peer.hpp
    source/interface.cpp
    include/network/interface.hpp
)

#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LIBS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)


add_subdirectory(test)
