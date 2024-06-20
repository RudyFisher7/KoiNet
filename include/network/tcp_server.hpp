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


#ifndef KOI_NETWORK_TCP_SERVER_HPP
#define KOI_NETWORK_TCP_SERVER_HPP


#include "network/peer.hpp"

#include "network/typedefs.hpp"
#include "network/enums.hpp"

#include <set>


namespace Koi { namespace Network {

class TcpServer : public Peer {
public:
    //

protected:
    std::set<Socket> _remote_handles;

public:
    //todo:: support socket options
    TcpServer(const char* hostname, const char* service, int connection_queue_size);

    TcpServer(const TcpServer& rhs) = delete;
    TcpServer(TcpServer&& rhs) = delete;

    ~TcpServer();

    TcpServer& operator=(const TcpServer& rhs) = delete;
    TcpServer& operator=(TcpServer&& rhs) = delete;

    int get_readiness() const override;
    bool is_new_connection_ready() const;

    //todo:: iterator getter

    Socket accept_new_connection(int select_flags);
    void remove_connection(Socket handle);

    SendReceiveResult receive(Socket origin_handle, char* out_buffer, BufferSize buffer_size, int flags);
    SendReceiveResult send(Socket destination_handle, const char* buffer, BufferSize buffer_size, int flags);
    SendReceiveResult relay(Socket origin_handle, const char* buffer, BufferSize buffer_size, int flags);
};

}
}


#endif //KOI_NETWORK_TCP_SERVER_HPP
