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


#ifndef KOI_NETWORK_TYPEDEFS_HPP
#define KOI_NETWORK_TYPEDEFS_HPP

#include "network/os/system_includes.hpp"

namespace Koi { namespace Network {

#if defined(_WIN32)
typedef SOCKET Socket;
typedef int SendReceiveResult;
typedef int BufferSize;
typedef size_t SocketAddressSize;
#else
typedef int Socket;
typedef ssize_t SendReceiveResult;
typedef size_t BufferSize;
constexpr const Socket INVALID_SOCKET = -1;
constexpr const Socket SOCKET_ERROR = -1;
typedef socklen_t SocketAddressSize;
#endif

typedef addrinfo AddressInfo;
typedef sockaddr SocketAddress;
typedef sockaddr_storage SocketAddressStorage;
typedef fd_set SocketSet;
typedef timeval TimeValue;

}
}

#endif //KOI_NETWORK_TYPEDEFS_HPP
