//
// Created by rfish on 5/17/2024.
//


#include "network/socket_peer.hpp"
#include "network/interface.hpp"

#include <vector>
#include <iostream>

int main() {

    Koi::Network::SocketPeer peer;

    std::vector<Koi::Network::Interface> interfaces;
    Koi::Network::SocketPeer::get_interfaces(interfaces);

    for (Koi::Network::Interface& i : interfaces) {
        std::cout << i << std::endl;
    }

    return 0;
}
