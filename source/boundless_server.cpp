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


#include "../include/network/boundless_server.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"
#include "../include/network/log/log.hpp"


namespace Koi { namespace Network {

BoundlessServer::BoundlessServer(const char* hostname, const char* service, int select_flags) {
    (void)open_handle(hostname, service, select_flags);
}


BoundlessServer::~BoundlessServer() {
    (void)close_handle();
}


Error BoundlessServer::open_handle(const char* hostname, const char* service, int select_flags) {
    Error result = NETWORK_ERROR_OK;

    if (_is_opened) {
        return NETWORK_ERROR_ALREADY;
    }

    bool was_successful = false;
    int operation_result = 0;
    int get_address_info_result = 0;

    AddressInfo hints = Internal::get_clean_address_info();
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    AddressInfo* bind_address = nullptr;
    get_address_info_result = Internal::get_address_info(hostname, service, &hints, &bind_address);
    was_successful = get_address_info_result == 0;
    KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);

    if (was_successful) {
        _handle = Internal::create_handle(*bind_address);
        was_successful = _handle != INVALID_SOCKET;
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        int option = 0;
        operation_result = Internal::set_handle_option(_handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option));
        was_successful = operation_result == 0;
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        result = Internal::bind_locally(_handle, bind_address->ai_addr, bind_address->ai_addrlen);
        was_successful = result == NETWORK_ERROR_OK;
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        Manager::get_singleton().add_handle_for(_handle, NETWORK_SELECT_FLAG_READ);
        KOI_NET_ASSERT(was_successful, Internal::print_last_error_string);
    }

    if (!was_successful && _handle != INVALID_SOCKET) {
        operation_result = Internal::close_handle(_handle);
        KOI_NET_ASSERT(operation_result == 0, Internal::print_last_error_string);
    }

    if (!was_successful) {
        result = Internal::get_last_error();
    } else {
        _is_opened = true;
    }

    if (get_address_info_result == 0 && bind_address != nullptr) {
        Internal::free_address_info(bind_address);
    }

    return result;
}

}
}