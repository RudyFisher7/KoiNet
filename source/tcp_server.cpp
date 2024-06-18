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


#include "network/tcp_server.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"
#include "../include/network/log/log.hpp"


namespace Koi { namespace Network {

TcpServer::TcpServer(const char* hostname, const char* service, int connection_queue_size) : Peer() {
    AddressInfo hints = Internal::get_clean_address_info();
    hints.ai_socktype = SOCK_STREAM;

    AddressInfo* bind_address = nullptr;
    Internal::get_address_info(hostname, service, &hints, &bind_address);
    _local_handle = Internal::create_handle(*bind_address);

    int option = 0;
    Internal::set_handle_option(_local_handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option));

    Internal::bind_locally(_local_handle, bind_address->ai_addr, bind_address->ai_addrlen);

    Internal::free_address_info(bind_address);

    Internal::listen_on_handle(_local_handle, connection_queue_size);

    Manager::get_singleton().add_handle_for(_local_handle, NETWORK_SELECT_FLAG_READ);
}


bool TcpServer::is_ready_to_send() const {
    bool result = true;

    auto it = _remote_handles.cbegin();
    auto end = _remote_handles.cend();

    while (result && it != end) {
        result = (Manager::get_singleton().get_handle_readiness(*it) & NETWORK_SELECT_FLAG_READ) > 0;

        KOI_NET_LOG(*it << " - NETWORK_SELECT_FLAG_READ = " << result);

        ++it;
    }

    return result;
}


bool TcpServer::is_data_ready_to_receive() const {
    bool result = true;

    auto it = _remote_handles.cbegin();
    auto end = _remote_handles.cend();

    while (result && it != end) {
        result = (Manager::get_singleton().get_handle_readiness(*it) & NETWORK_SELECT_FLAG_WRITE) > 0;

        KOI_NET_LOG(*it << " - NETWORK_SELECT_FLAG_WRITE = " << result);

        ++it;
    }

    return result;
}


bool TcpServer::is_exception_active() const {
    bool result = false;

    auto it = _remote_handles.cbegin();
    auto end = _remote_handles.cend();

    while (!result && it != end) {
        result = (Manager::get_singleton().get_handle_readiness(*it) & NETWORK_SELECT_FLAG_EXCEPTION) > 0;

        KOI_NET_LOG(*it << " - NETWORK_SELECT_FLAG_EXCEPTION = " << result);

        ++it;
    }

    return result;
}


bool TcpServer::is_new_connection_ready() const {
    bool result = false;

    result = (Manager::get_singleton().get_handle_readiness(_local_handle) & NETWORK_SELECT_FLAG_READ) > 0;

    KOI_NET_LOG(_local_handle << " - is ready for new connection = " << result);

    return result;
}


Socket TcpServer::accept_new_connection(int select_flags) {
    Socket result = INVALID_SOCKET;

    SocketAddressStorage storage {};
    SocketAddressSize storage_size = sizeof(storage);
    result = Internal::accept_on_handle(_local_handle, (SocketAddress*)&storage, &storage_size);

    _remote_handles.insert(result);

    Manager::get_singleton().add_handle_for(result, select_flags);

    return result;
}

}
}