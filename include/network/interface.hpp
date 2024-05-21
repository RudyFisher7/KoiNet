//
// Created by rfish on 5/17/2024.
//

#ifndef NETWORK_INTERFACE_HPP
#define NETWORK_INTERFACE_HPP


#include <string>


namespace Koi::Network {

struct Interface {
public:
    std::string ipv_4_unicast_address;
    std::string ipv_6_unicast_address;
};

} // Koi::Network

#endif //NETWORK_INTERFACE_HPP
