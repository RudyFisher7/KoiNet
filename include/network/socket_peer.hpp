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


    static const std::string WILD_CARD_ADDRESS;


private:
    static unsigned long _number_of_instances;

    int _last_error = SOCKET_PEER_ERROR_OK;

    int _mode = SOCKET_PEER_MODE_INVALID;
    int _protocol = SOCKET_PEER_PROTOCOL_INVALID;
    std::string _address;

    SOCKET _socket_handle = 0;

public:
    //fixme:: don't use stl
    static int get_interfaces(std::vector<Interface>& out_interfaces);


    SocketPeer();
    SocketPeer(Mode in_mode, Protocol in_protocol, std::string in_address);

    ~SocketPeer();


private:
    static bool _is_socket_valid(SOCKET socket_handle);
    static int _get_last_errno();

    void _startup();
    void _cleanup();

    int _socket();
    int _close();
};

}


#endif //NETWORKING_SOCKET_PEER_HPP
