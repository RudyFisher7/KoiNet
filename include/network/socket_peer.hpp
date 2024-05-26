#ifndef NETWORKING_SOCKET_PEER_HPP
#define NETWORKING_SOCKET_PEER_HPP


#include "network/interface.hpp"

#include <string>
#include <vector>




namespace Koi::Network {

#if defined(_WIN32)
#include <winsock2.h>
#else
#define INVALID_SOCKET -1
typedef int SOCKET;
#endif


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
    static unsigned long _number_of_instances;

    int _last_error = SOCKET_PEER_ERROR_OK;

    int _mode = SOCKET_PEER_MODE_INVALID;
    int _protocol = SOCKET_PEER_PROTOCOL_INVALID;

    SOCKET _socket_handle = INVALID_SOCKET;


public:
    static int get_interfaces(std::vector<Interface>& out_interfaces);


    SocketPeer();
    SocketPeer(Mode in_mode, Protocol in_protocol);

    ~SocketPeer();


private:
    void _startup();
    void _cleanup();

    int _socket();
};

}


#endif //NETWORKING_SOCKET_PEER_HPP
