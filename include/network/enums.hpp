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


/**
 * @brief Unified error codes.
 * @remarks Can be used to programmatically handle errors in a cross-platform
 * manner.
 * @see Internal::get_last_error_nonportable() and Internal::print_last_error_string()
 * for specific error information reported by underlying OS.
 */
enum Error : int {
    NETWORK_ERROR_MIN = 0,
    NETWORK_ERROR_OK = NETWORK_ERROR_MIN,

    // misc
    NETWORK_ERROR_NOT_INITIALIZED,
    NETWORK_ERROR_INVALID,
    NETWORK_ERROR_NO_RECOVERY,
    NETWORK_ERROR_FAULT,
    NETWORK_ERROR_ALREADY,
    NETWORK_ERROR_LOOP,
    NETWORK_ERROR_CHECK_SYSTEM_ERROR,

    // time-related
    NETWORK_ERROR_TRY_AGAIN,
    NETWORK_ERROR_TIMED_OUT,
    NETWORK_ERROR_WOULD_BLOCK,
    NETWORK_ERROR_IN_PROGRESS,
    NETWORK_ERROR_INTERRUPTED,

    // memory-related
    NETWORK_ERROR_BUFFER_OVERFLOW,
    NETWORK_ERROR_NO_BUFFER_SPACE,
    NETWORK_ERROR_NOT_ENOUGH_MEMORY,
    NETWORK_ERROR_ADDRESS_TOO_LONG,
    NETWORK_ERROR_MESSAGE_SIZE,

    // support-related
    NETWORK_ERROR_SERVICE_NOT_SUPPORTED,
    NETWORK_ERROR_SOCKET_TYPE_NOT_SUPPORTED,
    NETWORK_ERROR_FAMILY_NOT_SUPPORTED,
    NETWORK_ERROR_PROTOCOL_TYPE_NOT_SUPPORTED,
    NETWORK_ERROR_OPERATION_NOT_SUPPORTED,

    // connection-related
    NETWORK_ERROR_NETWORK_DOWN,
    NETWORK_ERROR_NETWORK_RESET,
    NETWORK_ERROR_NETWORK_UNREACHABLE,
    NETWORK_ERROR_NOT_CONNECTED,
    NETWORK_ERROR_IS_CONNECTED,
    NETWORK_ERROR_CONNECTION_REFUSED,
    NETWORK_ERROR_CONNECTION_RESET,
    NETWORK_ERROR_CONNECTION_ABORTED,
    NETWORK_ERROR_CONNECTION_SHUTDOWN,

    // host-related
    NETWORK_ERROR_HOST_UNRESOLVABLE,
    NETWORK_ERROR_HOST_UNREACHABLE,

    // address-related
    NETWORK_ERROR_ADDRESS_IN_USE,
    NETWORK_ERROR_ADDRESS_NOT_AVAILABLE,
    NETWORK_ERROR_ADDRESS_REQUIRED,

    // filesystem-related
    NETWORK_ERROR_NOT_A_FILE_OR_DIRECTORY,
    NETWORK_ERROR_READONLY_FILE_SYSTEM,

    // socket-related
    NETWORK_ERROR_NOT_A_SOCKET,
    NETWORK_ERROR_INVALID_SOCKET,
    NETWORK_ERROR_NO_MORE_SOCKETS_AVAILABLE,
    NETWORK_ERROR_NO_MORE_SOCKETS_AVAILABLE_IN_PROCESS,

    // permission/access-related
    NETWORK_ERROR_ACCESS_FORBIDDEN,
    NETWORK_ERROR_PERMISSION_DENIED,

    // function argument-related
    NETWORK_ERROR_OPTION_NOT_SUPPORTED,
    NETWORK_ERROR_OUT_OF_DOMAIN,

    // catch-all
    NETWORK_ERROR_UNKNOWN,
    NETWORK_ERROR_SIZE
};

}
}

#endif //KOI_NETWORK_ENUMS_HPP
