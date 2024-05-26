//
// Created by rfish on 5/25/2024.
//


#include "../include/network/socket_peer.hpp"


namespace Koi::Network {

unsigned long SocketPeer::_number_of_instances = 0ul;


SocketPeer::SocketPeer() {
    _startup();
}


SocketPeer::SocketPeer(Mode in_mode, Protocol in_protocol) :
        _mode(in_mode),
        _protocol(in_protocol) {
    _startup();
    _socket();
}


SocketPeer::~SocketPeer() {
    _cleanup();
}

}
