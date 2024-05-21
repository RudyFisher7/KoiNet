//
// Created by rfish on 5/17/2024.
//

#ifndef NETWORK_INTERFACE_HPP
#define NETWORK_INTERFACE_HPP


#include <string>
#include <iostream>


namespace Koi::Network {

struct Interface {
public:
    std::string friendly_name;
    std::string ipv_4_unicast_address;
    std::string ipv_6_unicast_address;
};

std::ostream& operator<<(std::ostream& lhs, const Interface& rhs);

} // Koi::Network

#endif //NETWORK_INTERFACE_HPP
