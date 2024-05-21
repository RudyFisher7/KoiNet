cmake_minimum_required(VERSION 3.27)
project(KoiNet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")


set(LIBS "")

set(SOURCES
    source/interface.cpp
)

set(HEADERS
    include/network/socket_peer.hpp
    include/network/interface.hpp
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LIBS
        ws2_32
        iphlpapi
    )

    list(APPEND SOURCES
        source/windows/socket_peer.cpp
    )
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND SOURCES
        source/unix/socket_peer.cpp
    )
else ()
    message(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} is not supported!")
endif ()

message(STATUS "LIBS: ${LIBS}")

add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LIBS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)


add_subdirectory(test)
