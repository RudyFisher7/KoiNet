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


#include "network/socket_selector.hpp"

#include "network/os/internal.hpp"


namespace Koi { namespace Network {

SocketSet SocketSelector::_master_read_set {};
SocketSet SocketSelector::_master_write_set {};
SocketSet SocketSelector::_master_exception_set {};

SocketSet SocketSelector::_read_set {};
SocketSet SocketSelector::_write_set {};
SocketSet SocketSelector::_exception_set {};

Socket SocketSelector::_largest_handle = 0;


Socket SocketSelector::get_largest_handle() {
    return _largest_handle;
}


int SocketSelector::add_handle_for(Socket handle, int flags) {
    int result = 0;

    if (SOCKET_HANDLER_FLAG_NONE != flags) {
        _set_largest_handle(handle);

        if ((SOCKET_HANDLER_FLAG_READ & flags) > 0) {
            Internal::set_socket_in_set(handle, &_master_read_set);
        }

        if ((SOCKET_HANDLER_FLAG_WRITE & flags) > 0) {
            Internal::set_socket_in_set(handle, &_master_write_set);
        }

        if ((SOCKET_HANDLER_FLAG_EXCEPTION & flags) > 0) {
            Internal::set_socket_in_set(handle, &_master_exception_set);
        }
    }

    return result;
}


int SocketSelector::select_handles(TimeValue* timeout) {
    int result = 0;

    if (_largest_handle > 0) {
        _read_set = _master_read_set;
        _write_set = _master_write_set;
        _exception_set = _master_exception_set;

        result = Internal::select_handles(
                _largest_handle + 1,
                &_read_set,
                &_write_set,
                &_exception_set,
                timeout
        );
    }

    return result;
}


int SocketSelector::get_handle_readiness(Socket handle) {
    int result = 0;

    if (Internal::is_socket_ready_in_set(handle, &_read_set)) {
        result |= SOCKET_HANDLER_FLAG_READ;
    }

    if (Internal::is_socket_ready_in_set(handle, &_write_set)) {
        result |= SOCKET_HANDLER_FLAG_WRITE;
    }

    if (Internal::is_socket_ready_in_set(handle, &_exception_set)) {
        result |= SOCKET_HANDLER_FLAG_EXCEPTION;
    }

    return result;
}


void SocketSelector::_set_largest_handle(Socket handle) {
    _largest_handle = _largest_handle < handle ? handle : _largest_handle;
}

}
}