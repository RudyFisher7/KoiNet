cmake_minimum_required(VERSION 3.27)
project(KoiNet)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")


set(LIBS "")

set(SOURCES
    source/interface.cpp
    source/common/internal.cpp
)

set(HEADERS
    include/network/internal.hpp
    include/network/interface.hpp
    include/network/enums.hpp
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LIBS
        ws2_32
        iphlpapi
    )

    list(APPEND SOURCES
        source/windows/internal.cpp
    )
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND SOURCES
        source/unix/internal.cpp
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
add_subdirectory(example)
