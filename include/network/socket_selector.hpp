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


#ifndef KOI_NETWORK_SOCKET_HANDLER_HPP
#define KOI_NETWORK_SOCKET_HANDLER_HPP


#include "network/typedefs.hpp"


namespace Koi { namespace Network {

class SocketSelector final {
public:
    enum Flag: int {
        SOCKET_HANDLER_FLAG_NONE = 0,
        SOCKET_HANDLER_FLAG_READ = 1 >> 0,
        SOCKET_HANDLER_FLAG_WRITE = 1 >> 1,
        SOCKET_HANDLER_FLAG_EXCEPTION = 1 >> 2
    };


private:
    static SocketSet _master_read_set;
    static SocketSet _master_write_set;
    static SocketSet _master_exception_set;

    static SocketSet _read_set;
    static SocketSet _write_set;
    static SocketSet _exception_set;

    static Socket _largest_handle;

public:
    static Socket get_largest_handle();

    static int add_handle_for(Socket handle, int flags);


    static int select_handles(TimeValue* timeout);


    static int get_handle_readiness(Socket handle);


private:
    static void _set_largest_handle(Socket handle);
};

}
}


#endif //KOI_NETWORK_SOCKET_HANDLER_HPP
