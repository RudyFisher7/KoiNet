/*
MIT License

Copyright (c) 2024 kiyasui-hito

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "network/os/internal.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/types.h>


namespace Koi { namespace Network {

int Internal::get_interfaces(std::vector<Interface>& out_interfaces) {
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
                getnameinfo(
                        address->ifa_addr, family_size, address_string, sizeof(address_string), 0, 0, NI_NUMERICHOST
                );
                out_interfaces.at(out_interfaces.size() - 1u).ipv_4_unicast_address = std::string(address_string);
            } else if (family == AF_INET6) {
                const int family_size = sizeof(sockaddr_in6);
                getnameinfo(
                        address->ifa_addr, family_size, address_string, sizeof(address_string), 0, 0, NI_NUMERICHOST
                );
                out_interfaces.at(out_interfaces.size() - 1u).ipv_6_unicast_address = std::string(address_string);
            }

            address = address->ifa_next;
        }
    }

    return result;
}


void Internal::startup() {
}


void Internal::cleanup() {
}


bool Internal::is_socket_valid(Socket socket_handle) {
    bool result = false;

    result = socket_handle >= 0;

    return result;
}


int Internal::close_handle(Socket handle) {
    int result = 0;

    result = close(handle);

    return result;
}


Error Internal::get_last_error() {
    return convert_system_error(errno);
}


int Internal::get_last_error_nonportable() {
    int result = 0;

    result = errno;

    return result;
}


Error Internal::convert_system_error(int system_error_value) {
    Error result = NETWORK_ERROR_UNKNOWN;

    switch (system_error_value) {
        case EWOULDBLOCK: // same as EAGAIN
            result = NETWORK_ERROR_WOULD_BLOCK;
            break;
        case EAI_AGAIN:
            result = NETWORK_ERROR_TRY_AGAIN;
            break;
        case EAI_BADFLAGS:
        case EINVAL:
            result = NETWORK_ERROR_INVALID;
            break;
        case EAI_FAIL:
            result = NETWORK_ERROR_NO_RECOVERY;
            break;
        case EAFNOSUPPORT:
        case EAI_ADDRFAMILY:
            result = NETWORK_ERROR_FAMILY_NOT_SUPPORTED;
            break;
        case EAI_MEMORY:
            result = NETWORK_ERROR_NOT_ENOUGH_MEMORY;
            break;
        case EAI_NONAME:
            result = NETWORK_ERROR_HOST_UNRESOLVABLE;
            break;
        case EAI_SERVICE:
            result = NETWORK_ERROR_SERVICE_NOT_SUPPORTED;
            break;
        case EOPNOTSUPP:
            result = NETWORK_ERROR_OPERATION_NOT_SUPPORTED;
            break;
        case EAI_SOCKTYPE:
            result = NETWORK_ERROR_SOCKET_TYPE_NOT_SUPPORTED;
            break;
        case EAI_OVERFLOW:
            result = NETWORK_ERROR_BUFFER_OVERFLOW;
            break;
        case EAI_SYSTEM:
            result = NETWORK_ERROR_CHECK_SYSTEM_ERROR;
        case EBADF:
            result = NETWORK_ERROR_INVALID_SOCKET;
            break;
        case ENOTSOCK:
            result = NETWORK_ERROR_NOT_A_SOCKET;
            break;
        case EADDRINUSE:
            result = NETWORK_ERROR_ADDRESS_IN_USE;
            break;
        case EDOM:
            result = NETWORK_ERROR_OUT_OF_DOMAIN;
            break;
        case EISCONN:
            result = NETWORK_ERROR_IS_CONNECTED;
            break;
        case ENOPROTOOPT:
            result = NETWORK_ERROR_OPTION_NOT_SUPPORTED;
            break;
        case EACCES:
            result = NETWORK_ERROR_ACCESS_FORBIDDEN;
            break;
        case ENAMETOOLONG:
            result = NETWORK_ERROR_ADDRESS_TOO_LONG;
            break;
        case ENOTDIR:
        case ENOENT:
            result = NETWORK_ERROR_NOT_A_FILE_OR_DIRECTORY;
            break;
        case ELOOP:
            result = NETWORK_ERROR_LOOP;
            break;
        case ENOMEM:
            result = NETWORK_ERROR_NO_BUFFER_SPACE;
            break;
        case EMFILE:
            result = NETWORK_ERROR_NO_MORE_SOCKETS_AVAILABLE_IN_PROCESS;
            break;
        case ENFILE:
            result = NETWORK_ERROR_NO_MORE_SOCKETS_AVAILABLE;
            break;
        case EROFS:
            result = NETWORK_ERROR_READONLY_FILE_SYSTEM;
            break;
        case ECONNREFUSED:
            result = NETWORK_ERROR_CONNECTION_REFUSED;
            break;
        case ECONNABORTED:
            result = NETWORK_ERROR_CONNECTION_ABORTED;
            break;
        case EALREADY:
            result = NETWORK_ERROR_ALREADY;
            break;
        case EPERM:
            result = NETWORK_ERROR_PERMISSION_DENIED;
            break;
        case ETIMEDOUT:
            result = NETWORK_ERROR_TIMED_OUT;
            break;
        case EPROTOTYPE:
            result = NETWORK_ERROR_PROTOCOL_TYPE_NOT_SUPPORTED;
            break;
        case ENETUNREACH:
            result = NETWORK_ERROR_NETWORK_UNREACHABLE;
            break;
        case EMSGSIZE:
            result = NETWORK_ERROR_MESSAGE_SIZE;
            break;
        case EDESTADDRREQ:
            result = NETWORK_ERROR_ADDRESS_REQUIRED;
            break;
        case EPIPE:
            result = NETWORK_ERROR_CONNECTION_SHUTDOWN;
            break;
        default:
            result = NETWORK_ERROR_UNKNOWN;
            break;
    }

    return result;
}

}
}
