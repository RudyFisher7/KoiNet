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


#include "../include/network/bound_client.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"
#include "../include/network/log/log.hpp"


namespace Koi { namespace Network {

BoundClient::BoundClient(const char* hostname, const char* service, int select_flags, Protocol protocol) {
    (void)open_handle(hostname, service, select_flags, protocol);
}


BoundClient::~BoundClient() {
    (void)close_handle();
}


int BoundClient::get_readiness() const {
    return Manager::get_singleton().get_handle_readiness(_handle);
}


Socket BoundClient::get_handle() const {
    return _handle;
}


bool BoundClient::is_connected() const {
    return _is_connected;
}


Error BoundClient::open_handle(const char* hostname, const char* service, int select_flags, Protocol protocol) {
    Error result = NETWORK_ERROR_OK;

    if (_is_connected || _is_opened) {
        return NETWORK_ERROR_ALREADY;
    }

    bool was_successful = false;
    int operation_result = 0;

    AddressInfo hints = Internal::get_clean_address_info();

    switch (protocol) {
        case NETWORK_PROTOCOL_STREAM:
            hints.ai_socktype = SOCK_STREAM;
            break;
        case NETWORK_PROTOCOL_DATAGRAM:
            hints.ai_socktype = SOCK_DGRAM;
            break;
        default:
            hints.ai_socktype = SOCK_STREAM;
            break;
    }

    result = Internal::get_address_info(hostname, service, &hints, &peer_address_info);
    was_successful = result == NETWORK_ERROR_OK;
    KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);

    if (was_successful) {
        _handle = Internal::create_handle(*peer_address_info);
        was_successful = _handle != INVALID_SOCKET;
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        int option = 0;
        operation_result = Internal::set_handle_option(_handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option));
        was_successful = operation_result == 0;
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        Manager::get_singleton().add_handle_for(_handle, select_flags);
    }

    if (!was_successful && _handle != INVALID_SOCKET) {
        operation_result = Internal::close_handle(_handle);
        KOI_NET_LOG_IF_NOT(operation_result == 0, Internal::print_last_error_string);
    }

    if (!was_successful) {
        result = Internal::get_last_error();
    } else {
        _is_opened = true;
    }

    return result;
}


Error BoundClient::close_handle() {
    Error result = NETWORK_ERROR_OK;
    int close_result = 0;

    Manager::get_singleton().remove_handle_from(_handle, NETWORK_SELECT_FLAG_ALL_SETS);

    if (peer_address_info != nullptr) {
        Internal::free_address_info(peer_address_info);
    }

    if (_handle != INVALID_SOCKET) {
        close_result = Internal::close_handle(_handle);
        KOI_NET_LOG_IF_NOT(close_result == 0, Internal::print_last_error_string);
        _is_opened = false;
        _is_connected = false;
    }

    if (close_result != 0) {
        result = Internal::get_last_error();
    }
    else
    {
        _handle = INVALID_SOCKET;
    }

    return result;
}


Error BoundClient::connect() {
    Error result = NETWORK_ERROR_NOT_A_SOCKET;
    bool can_connect = (
            _handle != INVALID_SOCKET
            && _is_opened
            && peer_address_info != nullptr
            && !_is_connected
    );

    if (can_connect) {
        result = Internal::bind_remotely(_handle, peer_address_info->ai_addr, peer_address_info->ai_addrlen);
        KOI_NET_LOG_IF_NOT(result == NETWORK_ERROR_OK, Internal::print_last_error_string);
    }

    if (result == NETWORK_ERROR_OK) {
        _is_connected = true;
    }

    return result;
}


Error BoundClient::disconnect() {
    Error result = NETWORK_ERROR_OK;
    return result;
}


SendReceiveResult BoundClient::send(const char* buffer, BufferSize buffer_size, int flags) {
    SendReceiveResult result = -1;

    result = Internal::send_over_bound_handle(
            _handle,
            buffer,
            buffer_size,
            flags
    );

    KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);

    return result;
}


SendReceiveResult BoundClient::receive(char* out_buffer, BufferSize buffer_size, int flags) {
    SendReceiveResult result = -1;

    result = Internal::receive_over_bound_handle(_handle, out_buffer, buffer_size, flags);
    KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);

    return result;
}

}
}