#include "../include/network/socket_peer.hpp"


#include <arpa/inet.h>
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Koi::Network {

int SocketPeer::get_interfaces(std::vector<Interface>& out_interfaces) {
    int result = 0;

    ifaddrs* addresses;
    if (getifaddrs(&addresses) == 0) {
        ifaddrs* address = addresses;
        while (address) {
            out_interfaces.emplace_back();

            out_interfaces.at(out_interfaces.size() - 1u).friendly_name = std::string(address->ifa_name);

            char address_string[100];
            int family = address->ifa_addr->sa_family;
            if (family == AF_INET) {
                const int family_size = sizeof(sockaddr_in);
                getnameinfo(address->ifa_addr, family_size, address_string, sizeof(address_string), 0, 0, NI_NUMERICHOST);
                out_interfaces.at(out_interfaces.size() - 1u).ipv_4_unicast_address = std::string(address_string);
            } else if (family == AF_INET6) {
                const int family_size = sizeof(sockaddr_in6);
                getnameinfo(address->ifa_addr, family_size, address_string, sizeof(address_string), 0, 0, NI_NUMERICHOST);
                out_interfaces.at(out_interfaces.size() - 1u).ipv_6_unicast_address = std::string(address_string);
            }

            address = address->ifa_next;
        }
    }

    return result;
}


bool SocketPeer::_is_socket_valid(SOCKET socket_handle) {
    bool result = false;

    result = socket_handle >= 0;

    return result;
}


int SocketPeer::_get_last_errno() {
    int result = 0;

    result = errno;

    return result;
}


void SocketPeer::_startup() {
    ++_number_of_instances;
}


void SocketPeer::_cleanup() {
    --_number_of_instances;
}


int SocketPeer::_close() {
    int result = 0;

    result = close(_socket_handle);

    return result;
}

}
