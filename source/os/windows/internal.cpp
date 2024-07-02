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

#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <codecvt>


namespace Koi { namespace Network {

static WSADATA _wsa_data;
static bool _is_wsa_started = false;


int Internal::get_interfaces(std::vector<Interface>& out_interfaces) {
    int result = 0;

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

        size_t name_length = wcslen(adapter->FriendlyName) * 2 + 1;
        char* name = new char[name_length];
        size_t result_chars = 0;
        wcstombs_s(&result_chars, name, name_length, adapter->FriendlyName, _TRUNCATE);
        out_interfaces.at(out_interfaces.size() - 1u).friendly_name = std::string(name);
//        std::wstring wide_name = std::wstring(adapter->FriendlyName);
//        out_interfaces.at(out_interfaces.size() - 1u).friendly_name = std::string(wide_name.begin(), wide_name.end());//fixme:: loss of data

        PIP_ADAPTER_UNICAST_ADDRESS unicast_address = adapter->FirstUnicastAddress;
        while (unicast_address) {
            char address_string[100];
            error = getnameinfo(
                    unicast_address->Address.lpSockaddr, unicast_address->Address.iSockaddrLength, address_string,
                    sizeof(address_string), 0, 0, NI_NUMERICHOST
            );
            if (error == 0) {
                if (unicast_address->Address.lpSockaddr->sa_family == AF_INET) {
                    out_interfaces.at(out_interfaces.size() - 1u).ipv_4_unicast_address = std::string(address_string);
                } else if (unicast_address->Address.lpSockaddr->sa_family == AF_INET6) {
                    out_interfaces.at(out_interfaces.size() - 1u).ipv_6_unicast_address = std::string(address_string);
                }
            }

            unicast_address = unicast_address->Next;
        }

        adapter = adapter->Next;
    }

    if (adapters && error == ERROR_SUCCESS) {
        free(adapters);
    }

    //todo:: set result to an enum probably

    return result;
}


Error Internal::startup() {
    Error result = NETWORK_ERROR_OK;
    int error = 0;

    if (!_is_wsa_started) {
        error = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
        _is_wsa_started = error == 0;
    }

    if (error != 0) {
        result = convert_system_error(error);
    }

    return result;
}


Error Internal::cleanup() {
    Error result = NETWORK_ERROR_OK;
    int error = 0;

    if (_is_wsa_started) {
        error = WSACleanup();
        _is_wsa_started = false;
    }

    if (error != 0) {
        result = convert_system_error(error);
    }

    return result;
}


Error Internal::get_name_info(
        const SocketAddress* socket_address,
        SocketAddressSize socket_length,
        char* out_host,
        SocketAddressSize max_host_length,
        char* out_service,
        SocketAddressSize service_length,
        int flags
) {
    Error result = NETWORK_ERROR_OK;

    int error = getnameinfo(
            socket_address,
            static_cast<int>(socket_length),
            out_host,
            static_cast<DWORD>(max_host_length),
            out_service,
            static_cast<DWORD>(service_length),
            flags
    );

    if (error != 0) {
        result = convert_system_error(error);
    }

    return result;
}


int Internal::set_handle_option(
        Socket handle,
        int level,
        int option_name,
        const char* option_value,
        SocketAddressSize option_length
) {
    int result = 0;

    result = setsockopt(
            handle,
            level,
            option_name,
            option_value,
            static_cast<int>(option_length)
    );

    return result;
}


Error Internal::bind_locally(
        Socket handle,
        SocketAddress* address,
        SocketAddressSize address_length
) {
    Error result = NETWORK_ERROR_OK;

    int error = bind(handle, address, static_cast<int>(address_length));

    if (error != 0) {
        result = get_last_error();
    }

    return result;
}


Error Internal::bind_remotely(
        Socket handle,
        SocketAddress* address,
        SocketAddressSize address_length
) {
    Error result = NETWORK_ERROR_OK;

    int error = connect(handle, address, static_cast<int>(address_length));

    if (error != 0) {
        result = get_last_error();
    }

    return result;
}


Socket Internal::accept_on_handle(
        Socket handle,
        SocketAddress* address,
        SocketAddressSize* address_size
) {
    int size = static_cast<int>(*address_size);

    return accept(
            handle,
            address,
            &size
    );
}


SendReceiveResult Internal::send_to(
        Socket handle,
        const char* buffer,
        BufferSize buffer_size,
        int flags,
        const SocketAddress* destination_address,
        SocketAddressSize destination_address_size
) {
    SendReceiveResult result = 0;

    result = sendto(
            handle,
            buffer,
            buffer_size,
            flags,
            destination_address,
            static_cast<int>(destination_address_size)
    );

    return result;
}


SendReceiveResult Internal::receive_from(
        Socket handle,
        char* buffer,
        BufferSize buffer_size,
        int flags,
        SocketAddress* origin_address,
        SocketAddressSize* origin_address_size
) {
    SendReceiveResult result = 0;
    int size = static_cast<int>(*origin_address_size);

    result = recvfrom(
            handle,
            buffer,
            buffer_size,
            flags,
            origin_address,
            &size
    );

    return result;
}


int Internal::select_handles(
        Socket number_of_handles,
        SocketSet* read_handles,
        SocketSet* write_handles,
        SocketSet* exception_handles,
        TimeValue* timeout
) {
    int result = 0;

    result = select(
            static_cast<int>(number_of_handles),
            read_handles,
            write_handles,
            exception_handles,
            timeout
    );

    return result;
}


bool Internal::is_socket_valid(Socket socket_handle) {
    bool result = false;

    result = socket_handle != INVALID_SOCKET;

    return result;
}


int Internal::close_handle(Socket handle) {
    int result = 0;

    result = closesocket(handle);

    return result;
}


Error Internal::get_last_error() {
    return convert_system_error(WSAGetLastError());
}


void Internal::print_last_error_string() {
    char result[256];

    FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            WSAGetLastError(),
            0,
            result,
            256,
            nullptr
    );

    char* newline = strrchr(result, '\n');
    if (newline != nullptr) {
        *newline = 0;
    }

    std::cout << result << std::endl;
}


int Internal::get_last_error_nonportable() {
    int result = 0;

    result = WSAGetLastError();

    return result;
}


Error Internal::convert_system_error(int system_error_value) {
    Error result = NETWORK_ERROR_UNKNOWN;

    switch (system_error_value) {
        case WSANOTINITIALISED:
            result = NETWORK_ERROR_NOT_INITIALIZED;
            break;
        case WSAEWOULDBLOCK:
            result = NETWORK_ERROR_WOULD_BLOCK;
            break;
        case WSAEINPROGRESS:
            result = NETWORK_ERROR_IN_PROGRESS;
            break;
        case WSATRY_AGAIN:
            result = NETWORK_ERROR_TRY_AGAIN;
            break;
        case WSAEINVAL:
            result = NETWORK_ERROR_INVALID;
            break;
        case WSANO_RECOVERY:
            result = NETWORK_ERROR_NO_RECOVERY;
            break;
        case WSAEAFNOSUPPORT:
            result = NETWORK_ERROR_FAMILY_NOT_SUPPORTED;
            break;
        case WSA_NOT_ENOUGH_MEMORY:
            result = NETWORK_ERROR_NOT_ENOUGH_MEMORY;
            break;
        case WSAHOST_NOT_FOUND:
            result = NETWORK_ERROR_HOST_UNRESOLVABLE;
            break;
        case WSATYPE_NOT_FOUND:
            result = NETWORK_ERROR_SERVICE_NOT_SUPPORTED;
            break;
        case WSAEOPNOTSUPP:
            result = NETWORK_ERROR_OPERATION_NOT_SUPPORTED;
            break;
        case WSAESOCKTNOSUPPORT:
            result = NETWORK_ERROR_SOCKET_TYPE_NOT_SUPPORTED;
            break;
        case WSAEFAULT:
            result = NETWORK_ERROR_FAULT;
            break;
        case WSAENOBUFS:
            result = NETWORK_ERROR_NO_BUFFER_SPACE;
            break;
        case WSAENETDOWN:
            result = NETWORK_ERROR_NETWORK_DOWN;
            break;
        case WSAENETRESET:
            result = NETWORK_ERROR_NETWORK_RESET;
            break;
        case WSAECONNRESET:
            result = NETWORK_ERROR_CONNECTION_RESET;
            break;
        case WSAENOTCONN:
            result = NETWORK_ERROR_NOT_CONNECTED;
            break;
        case WSAECONNABORTED:
            result = NETWORK_ERROR_CONNECTION_ABORTED;
            break;
        case WSAESHUTDOWN:
            result = NETWORK_ERROR_CONNECTION_SHUTDOWN;
            break;
        case WSAEMSGSIZE:
            result = NETWORK_ERROR_MESSAGE_SIZE;
            break;
        case WSAENOPROTOOPT:
            result = NETWORK_ERROR_OPTION_NOT_SUPPORTED;
            break;
        case WSAEADDRINUSE:
            result = NETWORK_ERROR_ADDRESS_IN_USE;
            break;
        case WSAEADDRNOTAVAIL:
            result = NETWORK_ERROR_ADDRESS_NOT_AVAILABLE;
            break;
        case WSAENOTSOCK:
            result = NETWORK_ERROR_NOT_A_SOCKET;
            break;
        case WSAEACCES:
            result = NETWORK_ERROR_ACCESS_FORBIDDEN;
            break;
        case WSAEINTR:
            result = NETWORK_ERROR_INTERRUPTED;
            break;
        case WSAEALREADY:
            result = NETWORK_ERROR_ALREADY;
            break;
        case WSAEISCONN:
            result = NETWORK_ERROR_IS_CONNECTED;
            break;
        case WSAENETUNREACH:
            result = NETWORK_ERROR_NETWORK_UNREACHABLE;
            break;
        case WSAEHOSTUNREACH:
            result = NETWORK_ERROR_HOST_UNREACHABLE;
            break;
        case WSAETIMEDOUT:
            result = NETWORK_ERROR_TIMED_OUT;
            break;
        case WSAEMFILE:
            result = NETWORK_ERROR_NO_MORE_SOCKETS_AVAILABLE;
            break;
        case WSAEDESTADDRREQ:
            result = NETWORK_ERROR_ADDRESS_REQUIRED;
            break;
        default:
            result = NETWORK_ERROR_UNKNOWN;
            break;
    }

    return result;
}

}
}
