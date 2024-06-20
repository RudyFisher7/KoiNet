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


#include "../include/network/tcp_client.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"
#include "../include/network/log/log.hpp"


namespace Koi { namespace Network {

TcpClient::TcpClient(const char* hostname, const char* service, int select_flags) {
    bool was_successful = false;
    int operation_result = 0;
    int get_address_info_result = 0;

    AddressInfo hints = Internal::get_clean_address_info();
    hints.ai_socktype = SOCK_STREAM;
    get_address_info_result = Internal::get_address_info(hostname, service, &hints, &peer_address_info);
    was_successful = get_address_info_result == 0;
    KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);

    if (was_successful) {
        _handle = Internal::create_handle(*peer_address_info);
        was_successful = _handle != INVALID_SOCKET;
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        Manager::get_singleton().add_handle_for(_handle, select_flags);
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }

    if (!was_successful && _handle != INVALID_SOCKET) {
        operation_result = Internal::close_handle(_handle);
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }
}


TcpClient::~TcpClient() {
    Manager::get_singleton().remove_handle_from(_handle, NETWORK_SELECT_FLAG_ALL);

    if (peer_address_info != nullptr) {
        Internal::free_address_info(peer_address_info);
    }
}


int TcpClient::get_readiness() const {
    return Manager::get_singleton().get_handle_readiness(_handle);
}


Error TcpClient::connect() {
    Error result = Error::NETWORK_ERROR_OK;
    int operation_result = 0;
    bool can_connect = (
            _handle != INVALID_SOCKET
            && peer_address_info != nullptr
    );

    if (can_connect) {
        operation_result = Internal::bind_remotely(_handle, peer_address_info->ai_addr, peer_address_info->ai_addrlen);
        KOI_NET_ASSERT(operation_result == 0, Internal::print_last_error_string);
    }

    if (operation_result != 0) {
        result = Internal::get_last_error();
    }

    return result;
}

}
}