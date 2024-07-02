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


#include "../include/network/i_boundless_peer.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"
#include "../include/network/log/log.hpp"


namespace Koi { namespace Network {

IBoundlessPeer::~IBoundlessPeer() {
    (void)close_handle();
}


int IBoundlessPeer::get_readiness() const {
    return Manager::get_singleton().get_handle_readiness(_handle);
}


Socket IBoundlessPeer::get_handle() const {
    return _handle;
}


Error IBoundlessPeer::close_handle() {
    Error result = NETWORK_ERROR_OK;
    int close_result = 0;

    Manager::get_singleton().remove_handle_from(_handle, NETWORK_SELECT_FLAG_ALL_SETS);

    if (_handle != INVALID_SOCKET) {
        close_result = Internal::close_handle(_handle);
        KOI_NET_LOG_IF_NOT(close_result == 0, Internal::print_last_error_string);
        _is_opened = false;
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


SendReceiveResult IBoundlessPeer::send_to(
        const char* buffer,
        BufferSize buffer_size,
        int flags,
        const SocketAddressStorage* destination_address
) {
    SendReceiveResult result = -1;

    constexpr SocketAddressSize address_size = sizeof(SocketAddressStorage);

    result = Internal::send_to(
            _handle,
            buffer,
            buffer_size,
            flags,
            (SocketAddress*)destination_address,
            address_size
    );

    KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);

    return result;
}


SendReceiveResult IBoundlessPeer::receive_from(
        char* out_buffer,
        BufferSize buffer_size,
        int flags,
        SocketAddressStorage* origin_address
) {
    SendReceiveResult result = -1;

    SocketAddressSize address_size = sizeof(SocketAddressStorage);

    result = Internal::receive_from(
            _handle,
            out_buffer,
            buffer_size,
            flags,
            (SocketAddress*)origin_address,
            &address_size
    );


    KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);

    return result;
}

}
}