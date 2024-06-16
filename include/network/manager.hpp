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


#ifndef KOI_NETWORK_MANAGER_HPP
#define KOI_NETWORK_MANAGER_HPP


#include "network/typedefs.hpp"
#include "network/enums.hpp"


namespace Koi { namespace Network {

class Manager final {
private:
    static SocketSet _master_read_set;
    static SocketSet _master_write_set;
    static SocketSet _master_exception_set;

    static SocketSet _read_set;
    static SocketSet _write_set;
    static SocketSet _exception_set;

    static Socket _largest_handle;

public:

    /**
     * @brief Gets the socket handle with the largest value that has been added
     * to the SocketSelector in any of its underlying SocketSets.
     * @return The largest handle.
     */
    static Socket get_largest_handle();


    static void startup();
    static void cleanup();


    /**
     * @brief Adds the given socket handle to the SocketSelector's underlying
     * SocketSets based on the flags provided.
     * @param handle The socket handle.
     * @param select_flags The flags that determine with SocketSets the handle should
     * be added to.
     * This value should be a lor-ed value of SelectFlag enum values.
     */
    static void add_handle_for(Socket handle, int select_flags);


    /**
     * @brief Selects the handles of the underlying SocketSets.
     * @param timeout The timeout of the select operation.
     * @return The number of bits set across all underlying SocketSets, or -1 on
     * error (0 if timed out).
     */
    static int select_handles(TimeValue* timeout);


    /**
     * @brief Gets a lor-ed value of the readiness of the given socket value.
     * @param handle The socket handle to check readiness of.
     * @return The lor-ed readiness value. This will be lor-ed
     * SelectFlag enum values.
     */
    static int get_handle_readiness(Socket handle);


private:

    /**
     * @brief Sets the largest handle this SocketSelector has in any of its
     * underlying SocketSets.
     * @param handle The value to set it to if it is greater than the current
     * largest handle.
     */
    static void _set_largest_handle(Socket handle);
};

}
}


#endif //KOI_NETWORK_MANAGER_HPP
