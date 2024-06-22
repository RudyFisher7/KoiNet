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


#ifndef KOI_NETWORK_TCP_CLIENT_HPP
#define KOI_NETWORK_TCP_CLIENT_HPP


#include "network/i_peer.hpp"

#include "network/typedefs.hpp"
#include "network/enums.hpp"


namespace Koi { namespace Network {

class BoundClient : public IPeer {
protected:
    bool _is_connected = false;
    bool _is_opened = false;
    Socket _handle = INVALID_SOCKET;
    AddressInfo* peer_address_info = nullptr;

public:
    BoundClient() = default;
    //todo:: support socket options
    BoundClient(const char* hostname, const char* service, int select_flags, Protocol protocol);

    BoundClient(const BoundClient& rhs) = delete;
    BoundClient(BoundClient&& rhs) = delete;


    /**
     * @brief Destroys the server object, cleans up its memory, and attempts to
     * close its socket handle.
     * @see NOTES of https://www.man7.org/linux/man-pages/man2/close.2.html
     * @warning This destructor does not retry close()-ing the socket handle.
     * If the particular system requires a retry on NETWORK_ERROR_INTERRUPTED,
     * close_handle() can be used to ensure closure before destruction.
     */
    ~BoundClient();

    BoundClient& operator=(const BoundClient& rhs) = delete;
    BoundClient& operator=(BoundClient&& rhs) = delete;


    int get_readiness() const override;

    bool is_connected() const;

    //todo:: support socket options
    Error open_handle(const char* hostname, const char* service, int select_flags, Protocol protocol);
    Error close_handle();
    Error connect();


    //todo:: implement Internal::shutdown, etc. and call here
    Error disconnect();


    SendReceiveResult send(const char* buffer, BufferSize buffer_size, int flags);
    SendReceiveResult receive(char* out_buffer, BufferSize buffer_size, int flags);
};

}
}

#endif //KOI_NETWORK_TCP_CLIENT_HPP
