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


#include "network/i_peer.hpp"

#include "network/typedefs.hpp"
#include "network/enums.hpp"

#include <set>


namespace Koi { namespace Network {

class BoundServer : public IPeer {
public:
    typedef std::set<Socket>::const_iterator ConstIterator;


protected:
    bool _is_listening = false;
    Socket _local_handle = INVALID_SOCKET;
    std::set<Socket> _remote_handles;


public:
    BoundServer() = default;
    //todo:: support socket options
    BoundServer(const char* hostname, const char* service, int connection_queue_size, Protocol protocol);

    BoundServer(const BoundServer& rhs) = delete;
    BoundServer(BoundServer&& rhs) = delete;

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
    ~BoundServer();

    BoundServer& operator=(const BoundServer& rhs) = delete;
    BoundServer& operator=(BoundServer&& rhs) = delete;

    int get_readiness() const override;
    Socket get_local_handle() const;
    Socket get_first_remote_socket() const;
    Socket get_last_remote_socket() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    bool is_listening() const;
    bool is_new_connection_ready() const;

    //todo:: support socket options
    Error open_local_handle(const char* hostname, const char* service, int connection_queue_size, Protocol protocol);


    /**
     * @brief Attempts to close this object's local socket handle.
     * @remarks If no errors were reported by the OS, the handle will be cleared
     * from this object.
     * If an error did occur during closure, the local handle will be left alone
     * in case a retry is needed (see warning below).
     * @see NOTES of https://www.man7.org/linux/man-pages/man2/close.2.html
     * @warning This function does not retry closing the socket handle.
     * If the particular system requires a retry on NETWORK_ERROR_INTERRUPTED,
     * This behavior is subject to change once POSIX standard enforces a unified
     * behavior across all systems.
     * @return NETWORK_ERROR_OK on if no error was raised by the OS, otherwise,
     * a relevant NETWORK_ERROR_* enum value.
     */
    Error close_local_handle();

    Socket accept_new_connection(int select_flags);

    //fixme:: implement Internal::shutdown, etc. and call here
    /**
     * @brief Attempts to close a remote socket handle.
     * @remarks If no errors were reported by the OS, the handle will be cleared
     * from this object.
     * If an error did occur during closure, the handle will be left alone
     * in case a retry is needed (see warning below).
     * @see NOTES of https://www.man7.org/linux/man-pages/man2/close.2.html
     * @warning This function does not retry closing the socket handle.
     * If the particular system requires a retry on NETWORK_ERROR_INTERRUPTED,
     * This behavior is subject to change once POSIX standard enforces a unified
     * behavior across all systems.
     * @return NETWORK_ERROR_OK on if no error was raised by the OS, otherwise,
     * a relevant NETWORK_ERROR_* enum value.
     */
    Error close_connection(Socket handle);
    Error remove_remote_handle(Socket handle);


    SendReceiveResult send(Socket destination_handle, const char* buffer, BufferSize buffer_size, int flags);
    SendReceiveResult relay(Socket origin_handle, const char* buffer, BufferSize buffer_size, int flags);
    SendReceiveResult receive(Socket origin_handle, char* out_buffer, BufferSize buffer_size, int flags);
};

}
}


#endif //KOI_NETWORK_TCP_SERVER_HPP
