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


#include "network/system_includes.hpp"
#include "network/enums.hpp"
#include "network/typedefs.hpp"
#include "network/interface.hpp"

#include <string>
#include <vector>


namespace Koi { namespace Network {

class Internal final {
private:
    static Socket _largest_socket_handle;
    static int _last_error;

public:
    static void startup();


    static void cleanup();


    //fixme:: don't use stl
    static int get_interfaces(std::vector<Interface>& out_interfaces);


    static AddressInfo get_clean_address_info();


    static int get_address_info(
            const char* hostname,
            const char* port,
            AddressInfo* hints,
            AddressInfo** out_result
    );


    static int get_name_info(
            const SocketAddress* socket_address,
            SocketLength socket_length,
            char* out_host,
            SocketLength max_host_length,
            char* out_service,
            SocketLength service_length,
            int flags
    );


    static void free_address_info(AddressInfo* address);


    static Socket create_handle(AddressInfo& in_address_info);


    static int set_handle_option(
            Socket handle,
            int level,
            int option_name,
            const char* option_value,
            SocketLength option_length
    );


    //todo:: test the validity of this template
    template<typename T>
    static int set_handle_option(
            Socket handle,
            int level,
            int option_name,
            const T& option_value
    ) {
        int result = 0;

        result = setsockopt(
                handle,
                level,
                option_name,
                static_cast<const char*>(&option_value),
                sizeof(T)
        );

        return result;
    }


    static int bind_locally(
            Socket handle,
            SocketAddress* address,
            SocketLength address_length
    );


    static int bind_remotely(
            Socket handle,
            SocketAddress* address,
            SocketLength address_length
    );


    static int listen_on_handle(Socket handle, int queue_size);


    static Socket accept_on_handle(
            Socket handle,
            SocketAddress* address,
            SocketLength* address_length
    );


    static SendReceiveResult send_over_stream(
            Socket handle,
            const char* buffer,
            BufferSize buffer_size,
            int flags
    );


    static SendReceiveResult receive_over_stream(
            Socket handle,
            char* buffer,
            BufferSize buffer_size,
            int flags
    );


    static SendReceiveResult send_datagram();//todo:: implement
    static SendReceiveResult receive_datagram();//todo:: implement


    static Socket get_number_of_handles();


    static void clean_socket_set(SocketSet* set);


    static void set_socket_in_set(Socket handle, SocketSet* set);


    static void clear_socket_set(Socket handle, SocketSet* set);


    static bool is_socket_ready_in_set(Socket handle, SocketSet* set);


    static int select_handles(
            Socket number_of_handles,
            SocketSet* read_handles,
            SocketSet* write_handles,
            SocketSet* exception_handles,
            TimeValue* timeout
    );


    static bool is_socket_valid(Socket socket_handle);


    static int get_last_errno();


    static int close_handle(Socket handle);


    Internal() = delete;


    Internal(const Internal& rhs) = delete;


    Internal(Internal&& rhs) = delete;


    Internal& operator=(const Internal& rhs) = delete;


    Internal& operator=(Internal&& rhs) = delete;
};

}
}


#endif //KOI_NETWORK_SERVER_HPP
