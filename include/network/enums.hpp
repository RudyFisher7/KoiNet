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


#ifndef KOI_NETWORK_ENUMS_HPP
#define KOI_NETWORK_ENUMS_HPP

namespace Koi { namespace Network {

enum Error : int {
    SOCKET_PEER_ERROR_OK = 0,
    SOCKET_PEER_ERROR_PROTOCOL,
    SOCKET_PEER_ERROR_INVALID_SOCKET,
    SOCKET_PEER_ERROR_SIZE,
};

enum Mode : int {
    SOCKET_PEER_MODE_INVALID = -1,
    SOCKET_PEER_MODE_MIN = 0,
    SOCKET_PEER_MODE_CLIENT = SOCKET_PEER_MODE_MIN,
    SOCKET_PEER_MODE_SERVER,
    SOCKET_PEER_MODE_SIZE
};


enum Protocol : int {
    SOCKET_PEER_PROTOCOL_INVALID = -1,
    SOCKET_PEER_PROTOCOL_MIN = 0,
    SOCKET_PEER_PROTOCOL_STREAM = SOCKET_PEER_PROTOCOL_MIN,
    SOCKET_PEER_PROTOCOL_DATAGRAM,
    SOCKET_PEER_PROTOCOL_SIZE
};

}
}

#endif //KOI_NETWORK_ENUMS_HPP
