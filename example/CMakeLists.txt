cmake_minimum_required(VERSION 3.27)
project(KoiNetExample)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")


set(LIBS KoiNet)

message(STATUS "LIBS: ${LIBS}")


#set(KOI_NET_EXAMPLE_1 KoiNetExample1)
#
#add_executable(${KOI_NET_EXAMPLE_1}
#    time_server.cpp
#)
#
#target_link_libraries(${KOI_NET_EXAMPLE_1} PRIVATE
#    ${LIBS}
#)


set(KOI_NET_EXAMPLE_2 KoiNetExample2)

add_executable(${KOI_NET_EXAMPLE_2}
    tcp_client.cpp
)

target_link_libraries(${KOI_NET_EXAMPLE_2} PRIVATE
    ${LIBS}
)