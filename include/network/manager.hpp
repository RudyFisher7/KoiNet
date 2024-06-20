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

#include <mutex>


namespace Koi { namespace Network {

class Manager final {
private:
    SocketSet _master_read_set {};
    SocketSet _master_write_set {};
    SocketSet _master_exception_set {};

    SocketSet _read_set {};
    SocketSet _write_set {};
    SocketSet _exception_set {};

    Socket _largest_handle = 0;

    mutable std::mutex _master_read_set_mutex;
    mutable std::mutex _master_write_set_mutex;
    mutable std::mutex _master_exception_set_mutex;

    mutable std::mutex _read_set_mutex;
    mutable std::mutex _write_set_mutex;
    mutable std::mutex _exception_set_mutex;

    mutable std::mutex _largest_handle_mutex;

public:

    static Manager& get_singleton();

    Manager(const Manager& rhs) = delete;
    Manager(Manager&& rhs) = delete;

    Manager& operator=(const Manager& rhs) = delete;
    Manager& operator=(Manager&& rhs) = delete;

    /**
     * @brief Gets the socket handle with the largest value that has been added
     * to the SocketSelector in any of its underlying SocketSets.
     * @return The largest handle.
     */
    Socket get_largest_handle() const;


    void startup() const;
    void cleanup() const;


    /**
     * @brief Adds the given socket handle to the SocketSelector's underlying
     * SocketSets based on the flags provided.
     * @param handle The socket handle.
     * @param select_flags The flags that determine with SocketSets the handle should
     * be added to.
     * This value should be a lor-ed value of SelectFlag enum values.
     */
    void add_handle_for(Socket handle, int select_flags);
    void remove_handle_from(Socket handle, int select_flags);


    /**
     * @brief Selects the handles of the underlying SocketSets.
     * @param timeout The timeout of the select operation.
     * @return The number of bits set across all underlying SocketSets, or -1 on
     * error (0 if timed out).
     */
    int select_handles(TimeValue* timeout);


    /**
     * @brief Gets a lor-ed value of the readiness of the given socket value.
     * @param handle The socket handle to check readiness of.
     * @return The lor-ed readiness value. This will be lor-ed
     * SelectFlag enum values.
     */
    int get_handle_readiness(Socket handle);


private:
    Manager();
    ~Manager() = default;


    bool _is_handle_ready_in_set(const Socket& handle, SocketSet& set, std::mutex& set_mutex);


    /**
     * @brief Sets the largest handle this SocketSelector has in any of its
     * underlying SocketSets.
     * @param handle The value to set it to if it is greater than the current
     * largest handle.
     */
    void _set_largest_handle(Socket handle);

    void _set_select_set(
            SocketSet& select_set,
            const SocketSet& master_set,
            std::mutex& select_set_mutex,
            std::mutex& master_set_mutex
    );
};

}
}


#endif //KOI_NETWORK_MANAGER_HPP
