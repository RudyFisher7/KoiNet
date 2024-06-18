//
// Created by rfish on 6/15/2024.
//

#include "network/tcp_server.hpp"

#include "../include/network/manager.hpp"
#include "../include/network/os/internal.hpp"


namespace Koi { namespace Network {

TcpServer::TcpServer(int select_flags, int connection_queue_size) : Peer(select_flags) {
    AddressInfo hints = Internal::get_clean_address_info();
    hints.ai_socktype = SOCK_STREAM;

    AddressInfo* bind_address = nullptr;
    Internal::get_address_info(nullptr, nullptr, &hints, &bind_address);
    _handle = Internal::create_handle(*bind_address);

    int option = 0;
    Internal::set_handle_option(_handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option));

    Internal::bind_locally(_handle, bind_address->ai_addr, bind_address->ai_addrlen);

    Internal::free_address_info(bind_address);

    Internal::listen_on_handle(_handle, connection_queue_size);

    Manager::get_singleton().add_handle_for(_handle, select_flags);
}


bool TcpServer::is_ready_to_send() const {
    return false;
}


bool TcpServer::is_data_ready_to_receive() const {
    return false;
}


bool TcpServer::is_exception_active() const {
    return false;
}


bool TcpServer::is_new_connection_ready() const {
    return false;
}


Socket TcpServer::accept_new_connection(int select_flags) {
    Socket result = INVALID_SOCKET;

    SocketAddressStorage storage {};
    SocketAddressSize storage_size = sizeof(storage);
    result = Internal::accept_on_handle(_handle, (SocketAddress*)&storage, &storage_size);

    _remote_handles.insert(result);

    Manager::get_singleton().add_handle_for(result, select_flags);

    return result;
}

}
}