#ifndef KOI_NETWORK_SERVER_HPP
#define KOI_NETWORK_SERVER_HPP


#include "network/enums.hpp"
#include "network/interface.hpp"

#include <string>
#include <vector>


#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else //Unix
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif


#if defined(_WIN32)
#include <winsock2.h>
#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif
#else
typedef int SOCKET;
#endif


namespace Koi { namespace Network {

class Internal final {
private:
    static SOCKET _largest_socket_handle;
    static int _last_error;

public:
    static void startup();


    static void cleanup();


    //fixme:: don't use stl
    static int get_interfaces(std::vector<Interface>& out_interfaces);


    static addrinfo get_clean_addr_info();


    static int get_addr_info(
            const char* hostname,
            const char* port,
            addrinfo* hints,
            addrinfo** out_result
    );


    static int get_name_info(
            const sockaddr* socket_address,
            socklen_t socket_length,
            char* out_host,
            socklen_t max_host_length,
            char* out_service,
            socklen_t service_length,
            int flags
    );


    static void free_addr_info(addrinfo* address);


    static SOCKET create_handle(addrinfo& in_address_info);


    static int set_handle_option();//todo:: implement


    static int bind_locally(
            SOCKET handle,
            sockaddr* address,
            socklen_t address_length
    );


    static int bind_remotely(
            SOCKET handle,
            sockaddr* address,
            socklen_t address_length
    );


    static int send_over_stream();//todo:: implement
    static int receive_over_stream();//todo:: implement
    static int send_datagram();//todo:: implement
    static int receive_datagram();//todo:: implement


    static SOCKET get_number_of_handles();


    static void file_descriptor_zero(fd_set* set);


    static void file_descriptor_set(SOCKET handle, fd_set* set);


    static void file_descriptor_clear(SOCKET handle, fd_set* set);


    static bool is_file_descriptor_set(SOCKET handle, fd_set* set);


    static int select_handles(
            SOCKET number_of_handles,
            fd_set* read_handles,
            fd_set* write_handles,
            fd_set* exception_handles,
            timeval* timeout
    );


    static bool is_socket_valid(SOCKET socket_handle);


    static int get_last_errno();


    static int close_handle(SOCKET handle);


    Internal() = delete;


    Internal(const Internal& rhs) = delete;


    Internal(Internal&& rhs) = delete;


    Internal& operator=(const Internal& rhs) = delete;


    Internal& operator=(Internal&& rhs) = delete;
};

}
}


#endif //KOI_NETWORK_SERVER_HPP
