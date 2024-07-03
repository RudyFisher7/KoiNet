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


#ifndef KOI_NETWORK_BOUNDLESS_PEER_HPP
#define KOI_NETWORK_BOUNDLESS_PEER_HPP


#include "network/i_peer.hpp"

#include "network/typedefs.hpp"
#include "network/enums.hpp"


namespace Koi { namespace Network {

class IUnboundPeer : public IPeer {
protected:
    bool _is_opened = false;
    Socket _handle = INVALID_SOCKET;


public:
    IUnboundPeer() = default;

    IUnboundPeer(const IUnboundPeer& rhs) = delete;
    IUnboundPeer(IUnboundPeer&& rhs) = delete;


    /**
     * @brief Destroys the object, cleans up its memory, and attempts to
     * close its socket handle(s).
     * @remarks If no errors were reported by the OS, this object's underlying
     * handle(s) will be invalidated or cleared.
     * If an error did occur during closure, the handle(s) will be left alone
     * in case a retry is needed (see warning below).
     * @see NOTES of https://www.man7.org/linux/man-pages/man2/close.2.html
     * @warning This destructor does not retry closing the socket handle.
     * If the particular system requires a retry on NETWORK_ERROR_INTERRUPTED,
     * close_handle() can be used to ensure closure before destruction.
     * This behavior is subject to change once POSIX standard enforces a unified
     * behavior across all systems.
     */
    virtual ~IUnboundPeer();

    IUnboundPeer& operator=(const IUnboundPeer& rhs) = delete;
    IUnboundPeer& operator=(IUnboundPeer&& rhs) = delete;


    int get_readiness() const override;
    Socket get_handle() const;

    //todo:: support socket options
    virtual Error open_handle(const char* hostname, const char* service, int select_flags) = 0;


    /**
     * @brief Attempts to close its socket handle(s).
     * @remarks If no errors were reported by the OS, this object's underlying
     * handle(s) will be invalidated or cleared.
     * If an error did occur during closure, the handle(s) will be left alone
     * in case a retry is needed (see warning below).
     * @see NOTES of https://www.man7.org/linux/man-pages/man2/close.2.html
     * @warning This destructor does not retry closing the socket handle.
     * If the particular system requires a retry on NETWORK_ERROR_INTERRUPTED,
     * close_handle() can be used to ensure closure before destruction.
     * This behavior is subject to change once POSIX standard enforces a unified
     * behavior across all systems.
     * @return NETWORK_ERROR_OK on if no error was raised by the OS, otherwise,
     * a relevant NETWORK_ERROR_* enum value.
     */
    Error close_handle();


    SendReceiveResult send_to(
            const char* buffer,
            BufferSize buffer_size,
            int flags,
            const SocketAddressStorage* destination_address
    );


    SendReceiveResult receive_from(
            char* out_buffer,
            BufferSize buffer_size,
            int flags,
            SocketAddressStorage* origin_address
    );
};

}
}


#endif //KOI_NETWORK_BOUNDLESS_PEER_HPP
