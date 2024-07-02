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


#include "../include/network/bound_server.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"
#include "../include/network/log/log.hpp"


namespace Koi { namespace Network {

BoundServer::BoundServer(const char* hostname, const char* service, int connection_queue_size, Protocol protocol) {
    (void)open_local_handle(hostname, service, connection_queue_size, protocol);
}


BoundServer::~BoundServer() {
    (void)close_local_handle();

    Socket handle = get_first_remote_read_socket();//fixme:: duplicated code
    Socket end = get_last_remote_read_socket();
    while (handle != end) {
        (void)close_connection(handle);
        handle = get_first_remote_read_socket();
    }

    (void) close_connection(end);

    handle = get_first_remote_write_socket();
    end = get_last_remote_write_socket();
    while (handle != end) {
        (void)close_connection(handle);
        handle = get_first_remote_write_socket();
    }

    (void) close_connection(end);

    handle = get_first_remote_exception_socket();
    end = get_last_remote_exception_socket();
    while (handle != end) {
        (void)close_connection(handle);
        handle = get_first_remote_exception_socket();
    }

    (void) close_connection(end);
}


int BoundServer::get_readiness() const {
    int result = 0;

    auto it_read = _remote_read_handles.cbegin();
    auto end_read = _remote_read_handles.cend();

    //fixme:: duplicated code
    while ((result & NETWORK_SELECT_FLAG_READ) == 0 && it_read != end_read) {
        result = Manager::get_singleton().get_handle_readiness(*it_read);
        KOI_NET_LOG(std::to_string(*it_read) + " - NETWORK_SELECT_FLAG_READ = " + std::to_string(result));
        ++it_read;
    }

    auto it_write = _remote_write_handles.cbegin();
    auto end_write = _remote_write_handles.cend();

    while ((result & NETWORK_SELECT_FLAG_WRITE) == 0 && it_write != end_write) {
        result = Manager::get_singleton().get_handle_readiness(*it_write);
        KOI_NET_LOG(std::to_string(*it_write) + " - NETWORK_SELECT_FLAG_WRITE = " + std::to_string(result));
        ++it_write;
    }

    auto it_exception = _remote_exception_handles.cbegin();
    auto end_exception = _remote_exception_handles.cend();

    while ((result & NETWORK_SELECT_FLAG_EXCEPTION) == 0 && it_exception != end_exception) {
        result = Manager::get_singleton().get_handle_readiness(*it_exception);
        KOI_NET_LOG(std::to_string(*it_exception) + " - NETWORK_SELECT_FLAG_EXCEPTION = " + std::to_string(result));
        ++it_exception;
    }

    if (is_new_connection_ready()) {
        result |= NETWORK_SELECT_FLAG_ACCEPT_CONNECTION;
    }

    return result;
}


Socket BoundServer::get_local_handle() const {
    return _local_handle;
}


Socket BoundServer::get_first_remote_read_socket() const {
    return *_remote_read_handles.cbegin();
}


Socket BoundServer::get_last_remote_read_socket() const {
    return *_remote_read_handles.cend();
}


Socket BoundServer::get_first_remote_write_socket() const {
    return *_remote_write_handles.cbegin();
}


Socket BoundServer::get_last_remote_write_socket() const {
    return *_remote_write_handles.cend();
}


Socket BoundServer::get_first_remote_exception_socket() const {
    return *_remote_exception_handles.cbegin();
}


Socket BoundServer::get_last_remote_exception_socket() const {
    return *_remote_exception_handles.cend();
}


BoundServer::ConstIterator BoundServer::cbegin_read() const {
    return _remote_read_handles.cbegin();
}


BoundServer::ConstIterator BoundServer::cend_read() const {
    return _remote_read_handles.cend();
}


BoundServer::ConstIterator BoundServer::cbegin_write() const {
    return _remote_write_handles.cbegin();
}


BoundServer::ConstIterator BoundServer::cend_write() const {
    return _remote_write_handles.cend();
}


BoundServer::ConstIterator BoundServer::cbegin_exception() const {
    return _remote_exception_handles.cbegin();
}


BoundServer::ConstIterator BoundServer::cend_exception() const {
    return _remote_exception_handles.cend();
}


bool BoundServer::is_listening() const {
    return _local_handle != INVALID_SOCKET;
}


bool BoundServer::is_new_connection_ready() const {
    bool result = false;

    result = (Manager::get_singleton().get_handle_readiness(_local_handle) & NETWORK_SELECT_FLAG_READ) > 0;

    KOI_NET_LOG(std::to_string(_local_handle) + " - is ready for new connection = " + std::to_string(result));

    return result;
}


Error BoundServer::open_local_handle(const char* hostname, const char* service, int connection_queue_size, Protocol protocol) {
    Error result = NETWORK_ERROR_OK;

    if (_is_listening) {
        return NETWORK_ERROR_ALREADY;
    }

    bool was_successful = false;
    int operation_result = 0;
    int get_address_info_result = 0;

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

    AddressInfo* bind_address = nullptr;
    get_address_info_result = Internal::get_address_info(hostname, service, &hints, &bind_address);
    was_successful = get_address_info_result == 0;
    KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);

    if (was_successful) {
        _local_handle = Internal::create_handle(*bind_address);
        was_successful = _local_handle != INVALID_SOCKET;
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        int option = 0;
        operation_result = Internal::set_handle_option(_local_handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option));
        was_successful = operation_result == 0;
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        result = Internal::bind_locally(_local_handle, bind_address->ai_addr, bind_address->ai_addrlen);
        was_successful = result == NETWORK_ERROR_OK;
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        operation_result = Internal::listen_on_handle(_local_handle, connection_queue_size);
        was_successful = operation_result == 0;
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (was_successful) {
        Manager::get_singleton().add_handle_for(_local_handle, NETWORK_SELECT_FLAG_READ);
        KOI_NET_LOG_IF_NOT(was_successful, Internal::print_last_error_string);
    }

    if (!was_successful && _local_handle != INVALID_SOCKET) {
        operation_result = Internal::close_handle(_local_handle);
        KOI_NET_LOG_IF_NOT(operation_result == 0, Internal::print_last_error_string);
    }

    if (!was_successful) {
        result = Internal::get_last_error();
    } else {
        _is_listening = true;
    }

    if (get_address_info_result == 0 && bind_address != nullptr) {
        Internal::free_address_info(bind_address);
    }

    return result;
}


Error BoundServer::close_local_handle() {
    Error result = NETWORK_ERROR_OK;
    int close_result = 0;

    Manager::get_singleton().remove_handle_from(_local_handle, NETWORK_SELECT_FLAG_READ);

    if (_local_handle != INVALID_SOCKET) {
        close_result = Internal::close_handle(_local_handle);
        KOI_NET_LOG_IF_NOT(close_result == 0, Internal::print_last_error_string);
        _is_listening = false;
    }

    if (close_result != 0) {
        result = Internal::get_last_error();
    } else {
        _local_handle = INVALID_SOCKET;
    }

    return result;
}


Socket BoundServer::accept_new_connection(int select_flags) {
    Socket result = INVALID_SOCKET;

    SocketAddressStorage storage {};
    SocketAddressSize storage_size = sizeof(storage);
    result = Internal::accept_on_handle(_local_handle, (SocketAddress*)&storage, &storage_size);
    KOI_NET_LOG_IF_NOT(result != INVALID_SOCKET, Internal::print_last_error_string);

    if (result != INVALID_SOCKET) {
        Manager::get_singleton().add_handle_for(result, select_flags);

        if ((select_flags & NETWORK_SELECT_FLAG_READ) > 0) {
            _remote_read_handles.insert(result);
        }

        if ((select_flags & NETWORK_SELECT_FLAG_WRITE) > 0) {
            _remote_write_handles.insert(result);
        }

        if ((select_flags & NETWORK_SELECT_FLAG_EXCEPTION) > 0) {
            _remote_exception_handles.insert(result);
        }
    }

    return result;
}


Error BoundServer::close_connection(Socket handle) {
    Error result = NETWORK_ERROR_OK;
    int close_result = 0;

    if (handle != INVALID_SOCKET) {
        close_result = Internal::close_handle(handle);
        KOI_NET_LOG_IF_NOT(close_result == 0, Internal::print_last_error_string);
    }

    if (close_result != 0) {
        result = Internal::get_last_error();
    } else {
        (void)_remote_read_handles.erase(handle);
        (void)_remote_write_handles.erase(handle);
        (void)_remote_exception_handles.erase(handle);
    }

    return result;
}


Error BoundServer::remove_remote_handle(Socket handle) {
    Error result = NETWORK_ERROR_OK;
    size_t number_of_erased_handles = 0;

    number_of_erased_handles += _remote_read_handles.erase(handle);
    number_of_erased_handles += _remote_write_handles.erase(handle);
    number_of_erased_handles += _remote_exception_handles.erase(handle);

    if (number_of_erased_handles <= 0) {
        result = NETWORK_ERROR_OUT_OF_DOMAIN;
    }

    return result;
}


SendReceiveResult BoundServer::send(Socket destination_handle, const char* buffer, BufferSize buffer_size, int flags) {
    SendReceiveResult result = -1;

    result = Internal::send_over_bound_handle(
            destination_handle,
            buffer,
            buffer_size,
            flags
    );

    KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);

    return result;
}


SendReceiveResult BoundServer::relay(Socket origin_handle, const char* buffer, BufferSize buffer_size, int flags) {
    SendReceiveResult result = 0;

    auto it = _remote_write_handles.cbegin();
    auto end = _remote_write_handles.cend();

    while (result != SOCKET_ERROR && it != end) {
        if (origin_handle != *it) {
            result = Internal::send_over_bound_handle(
                    *it,
                    buffer,
                    buffer_size,
                    flags
            );

            KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);
            KOI_NET_LOG_IF_NOT(result != buffer_size, "Not all bytes were sent to " + std::to_string(*it));
        }

        ++it;
    }

    return result;
}


SendReceiveResult BoundServer::receive(Socket origin_handle, char* out_buffer, BufferSize buffer_size, int flags) {
    SendReceiveResult result = -1;

    result = Internal::receive_over_bound_handle(origin_handle, out_buffer, buffer_size, flags);
    KOI_NET_LOG_IF_NOT(result != SOCKET_ERROR, Internal::print_last_error_string);

    return result;
}

}
}