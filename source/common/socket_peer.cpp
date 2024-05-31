//
// Created by rfish on 5/25/2024.
//


#include "../include/network/socket_peer.hpp"


namespace Koi::Network {

SOCKET SocketPeer::_largest_socket_handle = 0;
int SocketPeer::_last_error = SOCKET_PEER_ERROR_OK;


addrinfo SocketPeer::get_clean_addrinfo() {
    addrinfo result {};
    memset(&result, 0, sizeof(result));//fixme:: error handling?
    return result;
}


SOCKET SocketPeer::get_number_of_handles() {
    return _largest_socket_handle + 1;
}


int SocketPeer::select_sockets(
        SOCKET number_of_handles,
        fd_set* read_handles,
        fd_set* write_handles,
        fd_set* exception_handles,
        timeval* timeout
) {
    return select(number_of_handles, read_handles, write_handles, exception_handles, timeout);
}


SocketPeer::SocketPeer() {
}


SocketPeer::SocketPeer(const std::string& in_address, const std::string& in_port) :
        _address(in_address),
        _port(in_port) {
}


SocketPeer::~SocketPeer() {
    if (_local_addrinfo != nullptr) {
        freeaddrinfo(_local_addrinfo);
    }
}


SOCKET SocketPeer::get_local_handle() const {
    return _local_socket_handle;
}

}
