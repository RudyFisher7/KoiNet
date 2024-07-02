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


#include "../../../include/network/os/internal.hpp"

#include <cstring>
#include <ostream>


namespace Koi { namespace Network {

AddressInfo Internal::get_clean_address_info() {
    AddressInfo result {};
    memset(&result, 0, sizeof(result));
    return result;
}


Error Internal::get_address_info(
        const char* hostname,
        const char* port,
        AddressInfo* hints,
        AddressInfo** out_result
) {
    Error result = NETWORK_ERROR_OK;

    int error = getaddrinfo(
            hostname,
            port,
            hints,
            out_result
    );

    if (error != 0) {
        result = convert_system_error(error);
    }

    return result;
}


void Internal::free_address_info(AddressInfo* address) {
    if (address == nullptr) {
        return;
    }

    freeaddrinfo(address);
    address = nullptr;
}


Socket Internal::create_handle(AddressInfo& in_address_info) {
    Socket result = 0;

    result = socket(
            in_address_info.ai_family,
            in_address_info.ai_socktype,
            in_address_info.ai_protocol
    );

    return result;
}


int Internal::listen_on_handle(Socket handle, int queue_size) {
    int result = 0;

    result = listen(handle, queue_size);

    return result;
}


SendReceiveResult Internal::send_over_bound_handle(
        Socket handle,
        const char* buffer,
        BufferSize buffer_size,
        int flags
) {
    SendReceiveResult result = 0;

    result = send(handle, buffer, buffer_size, flags);

    return result;
}


SendReceiveResult Internal::receive_over_bound_handle(
        Socket handle,
        char* buffer,
        BufferSize buffer_size,
        int flags
) {
    SendReceiveResult result = 0;

    result = recv(handle, buffer, buffer_size, flags);

    return result;
}


void Internal::clean_socket_set(SocketSet* set) {
    FD_ZERO(set);
}


void Internal::set_socket_in_set(Socket handle, SocketSet* set) {
    FD_SET(handle, set);
}


void Internal::clear_socket_from_set(Socket handle, SocketSet* set) {
    FD_CLR(handle, set);
}


bool Internal::is_socket_ready_in_set(Socket handle, SocketSet* set) {
    return FD_ISSET(handle, set);
}

}
}
