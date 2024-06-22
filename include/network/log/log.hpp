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


#ifndef KOI_NETWORK_LOG_HPP
#define KOI_NETWORK_LOG_HPP

//fixme:: define in build system
#define ENABLE_KOI_NET_LOG 1

#ifdef ENABLE_KOI_NET_LOG

#include <iostream>
#include <string>


#define KOI_NET_LOG(message) Koi::Network::Logger::_koi_net_log(message);\
    KOI_NET_GET_FILE_AND_LINE_INFO(true)


#define KOI_NET_ASSERT(condition, arg) Koi::Network::Logger::_koi_net_assert(condition, arg);\
    KOI_NET_GET_FILE_AND_LINE_INFO(condition)


#define KOI_NET_GET_FILE_AND_LINE_INFO(condition)\
    if (!(condition)) {\
        std::cout << "\t\t\t" << __FILE__ << ":" << __LINE__ << std::endl;\
    }


namespace Koi { namespace Network {

class Logger final {
private:
    static const std::string _PREFIX;


public:
    static void _koi_net_log(const std::string& message);
    static void _koi_net_assert(bool condition, const std::string& message);
    static void _koi_net_assert(bool condition, void(*callback)());
};

}
}

#else

#define KOI_NET_LOG(message)
#define KOI_NET_ASSERT(condition, message)
#define KOI_NET_GET_FILE_AND_LINE_INFO(condition)

#endif

#endif //KOI_NETWORK_LOG_HPP
