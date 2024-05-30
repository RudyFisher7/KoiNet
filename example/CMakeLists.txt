cmake_minimum_required(VERSION 3.27)
project(KoiNetExample)


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


set(KOI_NET_EXAMPLE_1 KoiNetExample1)

add_executable(${KOI_NET_EXAMPLE_1}
    time_server.cpp
)

target_link_libraries(${KOI_NET_EXAMPLE_1} PRIVATE
    ${LIBS}
)


set(KOI_NET_EXAMPLE_2 KoiNetExample2)

add_executable(${KOI_NET_EXAMPLE_2}
    tcp_client.cpp
)

target_link_libraries(${KOI_NET_EXAMPLE_2} PRIVATE
    ${LIBS}
)