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


#include "network/manager.hpp"

#include "network/os/internal.hpp"


namespace Koi { namespace Network {


Manager& Manager::get_singleton() {
    static Manager _instance;
    return _instance;
}


Socket Manager::get_largest_handle() const {
    std::lock_guard<std::mutex> lock(_largest_handle_mutex);
    return _largest_handle;
}


void Manager::startup() const {
    Internal::startup();
}


void Manager::cleanup() const {
    Internal::cleanup();
}


void Manager::add_handle_for(Socket handle, int select_flags) {
    if (SOCKET_HANDLER_FLAG_NONE != select_flags) {
        _set_largest_handle(handle);

        if ((SOCKET_HANDLER_FLAG_READ & select_flags) > 0) {
            const std::lock_guard<std::mutex> lock(_master_read_set_mutex);
            Internal::set_socket_in_set(handle, &_master_read_set);
        }

        if ((SOCKET_HANDLER_FLAG_WRITE & select_flags) > 0) {
            const std::lock_guard<std::mutex> lock(_master_read_set_mutex);
            Internal::set_socket_in_set(handle, &_master_write_set);
        }

        if ((SOCKET_HANDLER_FLAG_EXCEPTION & select_flags) > 0) {
            const std::lock_guard<std::mutex> lock(_master_exception_set_mutex);
            Internal::set_socket_in_set(handle, &_master_exception_set);
        }
    }
}


int Manager::select_handles(TimeValue* timeout) {
    int result = 0;

    if (get_largest_handle() > 0) {
        _set_select_set(_read_set, _master_read_set, _read_set_mutex, _master_read_set_mutex);
        _set_select_set(_write_set, _master_write_set, _write_set_mutex, _master_write_set_mutex);
        _set_select_set(_exception_set, _master_exception_set, _exception_set_mutex, _master_exception_set_mutex);

        const std::lock_guard<std::mutex> read_lock(_read_set_mutex);
        const std::lock_guard<std::mutex> write_lock(_write_set_mutex);
        const std::lock_guard<std::mutex> exception_lock(_exception_set_mutex);

        result = Internal::select_handles(
                get_largest_handle() + 1,
                &_read_set,
                &_write_set,
                &_exception_set,
                timeout
        );
    }

    return result;
}


int Manager::get_handle_readiness(Socket handle) {
    int result = 0;

    if (_is_handle_ready_in_set(handle, _read_set, _read_set_mutex)) {
        result |= SOCKET_HANDLER_FLAG_READ;
    }

    if (_is_handle_ready_in_set(handle, _write_set, _write_set_mutex)) {
        result |= SOCKET_HANDLER_FLAG_WRITE;
    }

    if (_is_handle_ready_in_set(handle, _exception_set, _exception_set_mutex)) {
        result |= SOCKET_HANDLER_FLAG_EXCEPTION;
    }

    return result;
}


Manager::Manager() {
    const std::lock_guard<std::mutex> read_lock(_master_read_set_mutex);
    const std::lock_guard<std::mutex> write_lock(_master_write_set_mutex);
    const std::lock_guard<std::mutex> exception_lock(_master_exception_set_mutex);

    Internal::clean_socket_set(&_master_read_set);
    Internal::clean_socket_set(&_master_write_set);
    Internal::clean_socket_set(&_master_exception_set);
}


bool Manager::_is_handle_ready_in_set(const Socket& handle, SocketSet& set, std::mutex& set_mutex) {
    bool result = false;

    const std::lock_guard<std::mutex> read_lock(set_mutex);
    result = Internal::is_socket_ready_in_set(handle, &set);

    return result;
}


void Manager::_set_largest_handle(Socket handle) {
    const std::lock_guard<std::mutex> lock(_largest_handle_mutex);
    _largest_handle = _largest_handle < handle ? handle : _largest_handle;
}


void Manager::_set_select_set(
        SocketSet& select_set,
        const SocketSet& master_set,
        std::mutex& select_set_mutex,
        std::mutex& master_set_mutex
) {
    const std::lock_guard<std::mutex> master_lock(master_set_mutex);
    const std::lock_guard<std::mutex> select_lock(select_set_mutex);
    select_set = master_set;
}

}
}