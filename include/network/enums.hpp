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

enum SelectFlag: int {
    NETWORK_SELECT_FLAG_NONE = 0,
    NETWORK_SELECT_FLAG_READ = 1 >> 0,
    NETWORK_SELECT_FLAG_WRITE = 1 >> 1,
    NETWORK_SELECT_FLAG_EXCEPTION = 1 >> 2,
    NETWORK_SELECT_FLAG_ALL = 0xfffffff
};


enum Error : int {
    NETWORK_ERROR_MIN = 0,
    NETWORK_ERROR_OK = NETWORK_ERROR_MIN,
    NETWORK_ERROR_PROTOCOL,
    NETWORK_ERROR_INVALID_SOCKET,
    NETWORK_ERROR_SIZE
};


enum Mode : int {
    NETWORK_MODE_INVALID = -1,
    NETWORK_MODE_MIN = 0,
    NETWORK_MODE_CLIENT = NETWORK_MODE_MIN,
    NETWORK_MODE_SERVER,
    NETWORK_MODE_SIZE
};


enum Protocol : int {
    NETWORK_PROTOCOL_INVALID = -1,
    NETWORK_PROTOCOL_MIN = 0,
    NETWORK_PROTOCOL_STREAM = NETWORK_PROTOCOL_MIN,
    NETWORK_PROTOCOL_DATAGRAM,
    NETWORK_PROTOCOL_SIZE
};

}
}

#endif //KOI_NETWORK_ENUMS_HPP
