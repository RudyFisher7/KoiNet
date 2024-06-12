//
// Created by rfish on 5/17/2024.
//


#include "network/internal.hpp"
#include "network/interface.hpp"

#include <vector>
#include <iostream>

int main() {

    Koi::Network::Internal peer;

    std::vector<Koi::Network::Interface> interfaces;
    Koi::Network::Internal::get_interfaces(interfaces);

    for (Koi::Network::Interface& i : interfaces) {
        std::cout << i << std::endl;
    }

    return 0;
}
