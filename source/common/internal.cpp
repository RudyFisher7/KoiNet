//
// Created by rfish on 5/25/2024.
//


#include "../include/network/internal.hpp"

#include <cstring>


namespace Koi {
namespace Network {

SOCKET Internal::_largest_socket_handle = 0;
int Internal::_last_error = SOCKET_PEER_ERROR_OK;


addrinfo Internal::get_clean_addr_info() {
    addrinfo result {};
    memset(&result, 0, sizeof(result));//fixme:: error handling?
    return result;
}


int Internal::get_addr_info(
        const char* hostname,
        const char* port,
        addrinfo* hints,
        addrinfo** out_result
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
        const sockaddr* socket_address,
        socklen_t socket_length,
        char* out_host,
        socklen_t max_host_length,
        char* out_service,
        socklen_t service_length,
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


void Internal::free_addr_info(addrinfo* address) {
    freeaddrinfo(address);
    address = nullptr;
}


SOCKET Internal::create_handle(addrinfo& in_address_info) {
    SOCKET result = 0;

    result = socket(
            in_address_info.ai_family,
            in_address_info.ai_socktype,
            in_address_info.ai_protocol
    );

    return result;
}


int Internal::set_handle_option(
        SOCKET handle,
        int level,
        int option_name,
        const char* option_value,
        socklen_t option_length
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
        SOCKET handle,
        sockaddr* address,
        socklen_t address_length
) {
    int result = 0;

    result = bind(handle, address, address_length);

    return result;
}


int Internal::bind_remotely(
        SOCKET handle,
        sockaddr* address,
        socklen_t address_length
) {
    int result = 0;

    result = connect(handle, address, address_length);

    return result;
}


int Internal::listen_on_handle(SOCKET handle, int queue_size) {
    int result = 0;

    result = listen(handle, queue_size);

    return result;
}


SOCKET Internal::accept_on_handle(
        SOCKET handle,
        sockaddr* address,
        int* address_length
) {
    return accept(
            handle,
            address,
            address_length
    );
}


SOCKET Internal::get_number_of_handles() {
    return _largest_socket_handle + 1;
}


void Internal::file_descriptor_zero(fd_set* set) {
    FD_ZERO(set);
}


void Internal::file_descriptor_set(SOCKET handle, fd_set* set) {
    FD_SET(handle, set);
}


void Internal::file_descriptor_clear(SOCKET handle, fd_set* set) {
    FD_CLR(handle, set);
}


bool Internal::is_file_descriptor_set(SOCKET handle, fd_set* set) {
    return FD_ISSET(handle, set);
}


int Internal::select_handles(
        SOCKET number_of_handles,
        fd_set* read_handles,
        fd_set* write_handles,
        fd_set* exception_handles,
        timeval* timeout
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
