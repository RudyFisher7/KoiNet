cmake_minimum_required(VERSION 3.13)
project(KoiNetExample)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")


set(LIBS KoiNet)

message(STATUS "LIBS: ${LIBS}")


set(KOI_NET_UDP_CLIENT_EXAMPLE KoiNetUdpClientExample)

add_executable(${KOI_NET_UDP_CLIENT_EXAMPLE}
    udp_client.cpp
)

target_link_libraries(${KOI_NET_UDP_CLIENT_EXAMPLE} PRIVATE
    ${LIBS}
)


set(KOI_NET_TCP_CLIENT_EXAMPLE KoiNetTcpClientExample)

add_executable(${KOI_NET_TCP_CLIENT_EXAMPLE}
    tcp_client.cpp
)

target_link_libraries(${KOI_NET_TCP_CLIENT_EXAMPLE} PRIVATE
    ${LIBS}
)



set(KOI_NET_TCP_SERVER_EXAMPLE KoiNetTcpServerExample)

add_executable(${KOI_NET_TCP_SERVER_EXAMPLE}
    koi_tcp_server_example.cpp
)

target_link_libraries(${KOI_NET_TCP_SERVER_EXAMPLE} PRIVATE
    ${LIBS}
)