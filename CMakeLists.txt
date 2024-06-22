cmake_minimum_required(VERSION 3.13)
project(KoiNet VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")


set(LIBS "")

set(SOURCES
    source/log/log.cpp
    source/interface.cpp
    source/os/common/internal.cpp
    source/manager.cpp
    source/i_peer.cpp
    source/i_boundless_peer.cpp
    source/bound_client.cpp
    source/bound_server.cpp
    source/boundless_client.cpp
    source/boundless_server.cpp
)

set(HEADERS
    include/network/log/log.hpp
    include/network/os/system_includes.hpp
    include/network/os/internal.hpp
    include/network/typedefs.hpp
    include/network/enums.hpp
    include/network/interface.hpp
    include/network/manager.hpp
    include/network/i_peer.hpp
    include/network/i_boundless_peer.hpp
    include/network/bound_client.hpp
    include/network/bound_server.hpp
    include/network/boundless_client.hpp
    include/network/boundless_server.hpp
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LIBS
        ws2_32
        iphlpapi
    )

    list(APPEND SOURCES
        source/os/windows/internal.cpp
    )
else ()
    list(APPEND SOURCES
        source/os/unix/internal.cpp
    )
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
add_subdirectory(example)
