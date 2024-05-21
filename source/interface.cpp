//
// Created by rfish on 5/17/2024.
//

#include "network/interface.hpp"


namespace Koi::Network {

std::ostream& operator<<(std::ostream& lhs, const Interface& rhs) {
    lhs << "friendly name: " << rhs.friendly_name << "\n";
    lhs << "ipv4: " << rhs.ipv_4_unicast_address << "\n";
    lhs << "ipv6: " << rhs.ipv_6_unicast_address << "\n";

    return lhs;
}

} // Koi::Network
