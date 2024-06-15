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

#include <cstring>


namespace Koi {
namespace Network {

int Internal::_last_error = SOCKET_PEER_ERROR_OK;


AddressInfo Internal::get_clean_address_info() {
    AddressInfo result {};
    memset(&result, 0, sizeof(result));//fixme:: error handling?
    return result;
}


int Internal::get_address_info(
        const char* hostname,
        const char* port,
        AddressInfo* hints,
        AddressInfo** out_result
) {
    int result = 0;

    result = getaddrinfo(
            hostname,
            port,
            hints,
            out_result
    );
    //fixme:: error stuff

    return result;
}


int Internal::get_name_info(
        const SocketAddress* socket_address,
        SocketAddressSize socket_length,
        char* out_host,
        SocketAddressSize max_host_length,
        char* out_service,
        SocketAddressSize service_length,
        int flags
) {
    int result = 0;

    result = getnameinfo(
            socket_address,
            socket_length,
            out_host,
            max_host_length,
            out_service,
            service_length,
            flags
    );
    //fixme:: error stuff

    return result;
}


void Internal::free_address_info(AddressInfo* address) {
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
            option_length
    );

    return result;
}


int Internal::bind_locally(
        Socket handle,
        SocketAddress* address,
        SocketAddressSize address_length
) {
    int result = 0;

    result = bind(handle, address, address_length);

    return result;
}


int Internal::bind_remotely(
        Socket handle,
        SocketAddress* address,
        SocketAddressSize address_length
) {
    int result = 0;

    result = connect(handle, address, address_length);

    return result;
}


int Internal::listen_on_handle(Socket handle, int queue_size) {
    int result = 0;

    result = listen(handle, queue_size);

    return result;
}


Socket Internal::accept_on_handle(
        Socket handle,
        SocketAddress* address,
        SocketAddressSize* address_size
) {

    return accept(
            handle,
            address,
            address_size
    );
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
            destination_address_size
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

    result = recvfrom(
            handle,
            buffer,
            buffer_size,
            flags,
            origin_address,
            origin_address_size
    );

    return result;
}


void Internal::clean_socket_set(SocketSet* set) {
    FD_ZERO(set);
}


void Internal::set_socket_in_set(Socket handle, SocketSet* set) {
    FD_SET(handle, set);
}


void Internal::clear_socket_set(Socket handle, SocketSet* set) {
    FD_CLR(handle, set);
}


bool Internal::is_socket_ready_in_set(Socket handle, SocketSet* set) {
    return FD_ISSET(handle, set);
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
#if defined(_WIN32)
            static_cast<int>(number_of_handles),
#else
            number_of_handles,
#endif
            read_handles,
            write_handles,
            exception_handles,
            timeout
    );

    return result;
}

}
}
