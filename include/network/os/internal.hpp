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


#ifndef KOI_NETWORK_SERVER_HPP
#define KOI_NETWORK_SERVER_HPP


#include "network/os/system_includes.hpp"
#include "network/enums.hpp"
#include "network/typedefs.hpp"
#include "network/interface.hpp"

#include <string>
#include <vector>


namespace Koi { namespace Network {

class Internal final {
public:
    static void startup();


    static void cleanup();


    static int get_interfaces(std::vector<Interface>& out_interfaces);


    static AddressInfo get_clean_address_info();


    static Error get_address_info(
            const char* hostname,
            const char* port,
            AddressInfo* hints,
            AddressInfo** out_result
    );


    static Error get_name_info(
            const SocketAddress* socket_address,
            SocketAddressSize socket_length,
            char* out_host,
            SocketAddressSize max_host_length,
            char* out_service,
            SocketAddressSize service_length,
            int flags
    );


    static void free_address_info(AddressInfo* address);


    static Socket create_handle(AddressInfo& in_address_info);


    static int set_handle_option(
            Socket handle,
            int level,
            int option_name,
            const char* option_value,
            SocketAddressSize option_length
    );


    static Error bind_locally(
            Socket handle,
            SocketAddress* address,
            SocketAddressSize address_length
    );


    static Error bind_remotely(
            Socket handle,
            SocketAddress* address,
            SocketAddressSize address_length
    );


    static int listen_on_handle(Socket handle, int queue_size);


    static Socket accept_on_handle(
            Socket handle,
            SocketAddress* address,
            SocketAddressSize* address_size
    );


    static SendReceiveResult send_over_bound_handle(
            Socket handle,
            const char* buffer,
            BufferSize buffer_size,
            int flags
    );


    static SendReceiveResult receive_over_bound_handle(
            Socket handle,
            char* buffer,
            BufferSize buffer_size,
            int flags
    );


    static SendReceiveResult send_to(
            Socket handle,
            const char* buffer,
            BufferSize buffer_size,
            int flags,
            const SocketAddress* destination_address,
            SocketAddressSize destination_address_size
    );


    static SendReceiveResult receive_from(
            Socket handle,
            char* buffer,
            BufferSize buffer_size,
            int flags,
            SocketAddress* origin_address,
            SocketAddressSize* origin_address_size
    );


    static void clean_socket_set(SocketSet* set);


    static void set_socket_in_set(Socket handle, SocketSet* set);


    static void clear_socket_from_set(Socket handle, SocketSet* set);


    static bool is_socket_ready_in_set(Socket handle, SocketSet* set);


    static int select_handles(
            Socket number_of_handles,
            SocketSet* read_handles,
            SocketSet* write_handles,
            SocketSet* exception_handles,
            TimeValue* timeout
    );


    static bool is_socket_valid(Socket socket_handle);


    /**
     * @brief
     * @param handle
     * @see NOTES of https://www.man7.org/linux/man-pages/man2/close.2.html
     * @return
     */
    static int close_handle(Socket handle);


    static Error get_last_error();


    static void print_last_error_string();


    static int get_last_error_nonportable();


    Internal() = delete;


    Internal(const Internal& rhs) = delete;


    Internal(Internal&& rhs) = delete;


    Internal& operator=(const Internal& rhs) = delete;


    Internal& operator=(Internal&& rhs) = delete;

private:
    static Error convert_system_error(int system_error_value);
};

}
}


#endif //KOI_NETWORK_SERVER_HPP
