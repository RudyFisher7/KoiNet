#ifndef NETWORKING_SOCKET_PEER_HPP
#define NETWORKING_SOCKET_PEER_HPP


#include "network/interface.hpp"
#include "socket_peer.hpp"

#include <string>
#include <vector>


#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else //Unix
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif


#if defined(_WIN32)
#include <winsock2.h>
#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif
#else
typedef int SOCKET;
#endif


namespace Koi::Network {

class SocketPeer final {
public:
    enum Error: int {
        SOCKET_PEER_ERROR_OK = 0,
        SOCKET_PEER_ERROR_PROTOCOL,
        SOCKET_PEER_ERROR_INVALID_SOCKET,
        SOCKET_PEER_ERROR_SIZE,
    };

    enum Mode: int {
        SOCKET_PEER_MODE_INVALID = -1,
        SOCKET_PEER_MODE_MIN = 0,
        SOCKET_PEER_MODE_CLIENT = SOCKET_PEER_MODE_MIN,
        SOCKET_PEER_MODE_SERVER,
        SOCKET_PEER_MODE_SIZE
    };


    enum Protocol: int {
        SOCKET_PEER_PROTOCOL_INVALID = -1,
        SOCKET_PEER_PROTOCOL_MIN = 0,
        SOCKET_PEER_PROTOCOL_STREAM = SOCKET_PEER_PROTOCOL_MIN,
        SOCKET_PEER_PROTOCOL_DATAGRAM,
        SOCKET_PEER_PROTOCOL_SIZE
    };


private:
    static SOCKET _largest_socket_handle;
    static int _last_error;

    int _mode = SOCKET_PEER_MODE_INVALID;
    int _protocol = SOCKET_PEER_PROTOCOL_INVALID;
    std::string _address;
    std::string _port;

    SOCKET _local_socket_handle = 0;
    addrinfo _local_hints;
    addrinfo* _local_addrinfo = nullptr;

    SOCKET _remote_socket_handle = 0;
    addrinfo _remote_hints;
    addrinfo* _remote_addrinfo = nullptr;

public:
    static void startup();
    static void cleanup();

    //fixme:: don't use stl
    static int get_interfaces(std::vector<Interface>& out_interfaces);

    static addrinfo get_clean_addrinfo();


    static SOCKET get_number_of_handles();


    static int select_sockets(
            SOCKET number_of_handles,
            fd_set* read_handles,
            fd_set* write_handles,
            fd_set* exception_handles,
            timeval* timeout
    );


    SocketPeer();
    SocketPeer(const std::string& in_address, const std::string& in_port);

    SocketPeer(const SocketPeer& rhs) = delete;
    SocketPeer(SocketPeer&& rhs) = delete;

    SocketPeer& operator=(const SocketPeer& rhs) = delete;
    SocketPeer& operator=(SocketPeer&& rhs) = delete;

    ~SocketPeer();


    SOCKET get_local_handle() const;

    int open_local_handle(const addrinfo& hints);
    int open_remote_handle(const addrinfo& hints);
    int close_local_handle();
    int close_remote_handle();//fixme:: probably should make server a separate class since they support multiplexing

    int bind_locally();
    int bind_remotely();

private:
    static bool _is_socket_valid(SOCKET socket_handle);
    static int _get_last_errno();
};

}


#endif //NETWORKING_SOCKET_PEER_HPP
