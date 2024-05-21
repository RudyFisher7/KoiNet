#include "../include/network/socket_peer.hpp"


#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

namespace Koi::Network {

#if defined(_WIN32)
WSADATA SocketPeer::_wsa_data;
bool SocketPeer::_is_wsa_started = false;
unsigned long SocketPeer::_number_of_instances = 0u;
#endif


int SocketPeer::get_interfaces(std::vector<Interface>& out_interfaces) {
    int result = 0;

#if defined(_WIN32)
    ULONG error = ERROR_SUCCESS;

    DWORD address_size = 20000;

    PIP_ADAPTER_ADDRESSES adapters; // typedef'ed as a pointer type

    do {
        adapters = (PIP_ADAPTER_ADDRESSES) malloc(address_size);

        if (!adapters) {
            error = ERROR_NOT_ENOUGH_MEMORY;
        }

        if (error == 0l) {
            error = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &address_size);
        }

        if (error == ERROR_BUFFER_OVERFLOW) {
            free(adapters);
        } else if (error == ERROR_SUCCESS) {
            //
        }
    } while (!adapters && error == ERROR_BUFFER_OVERFLOW);

    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter) {
        out_interfaces.emplace_back();
        PIP_ADAPTER_UNICAST_ADDRESS unicast_address = adapter->FirstUnicastAddress;
        while (unicast_address) {
            char address_string[100];
            error = getnameinfo(unicast_address->Address.lpSockaddr, unicast_address->Address.iSockaddrLength, address_string, sizeof(address_string), 0, 0, NI_NUMERICHOST);
            if (error == 0) {
                if (unicast_address->Address.lpSockaddr->sa_family == AF_INET) {
                    out_interfaces.at(out_interfaces.size() - 1u).ipv_4_unicast_address = std::string(address_string);
                } else if (unicast_address->Address.lpSockaddr->sa_family == AF_INET6) {
                    out_interfaces.at(out_interfaces.size() - 1u).ipv_6_unicast_address = std::string(address_string);
                }
            }

            //FIXME:: RUDY! START VERSION CONTROL!!!!!!!

            unicast_address = unicast_address->Next;
        }

        adapter = adapter->Next;
    }

    if (adapters && error == ERROR_SUCCESS) {
        free(adapters);
    }
#else
    //todo::
#endif

    return result;
}


SocketPeer::SocketPeer() {
#if defined(_WIN32)
    if (!_is_wsa_started) {
        int error = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
        _is_wsa_started = error == 0;
        _last_error = error; //fixme:: use an enum value probably
    }

    if (_is_wsa_started) {
        ++_number_of_instances;
    }
#endif
}


SocketPeer::~SocketPeer() {
#if defined(_WIN32)
    if (_is_wsa_started) {
        --_number_of_instances;

        if (_number_of_instances == 0u) {
            (void)WSACleanup();
            _is_wsa_started = false;
        }
    }
#endif
}

}
