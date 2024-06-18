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


#include <network/manager.hpp>
#include <network/tcp_server.hpp>
#include <network/enums.hpp>


int main() {
    bool is_running = true;
    Koi::Network::Manager::get_singleton().startup();

    Koi::Network::TcpServer server("::1", "8080", 10);

    Koi::Network::TimeValue timeout { 0, 10000 };
    while(is_running) {
        if (Koi::Network::Manager::get_singleton().select_handles(&timeout) < 0) {
            is_running = false;
        }

        while (server.is_new_connection_ready()) {
            server.accept_new_connection(Koi::Network::NETWORK_SELECT_FLAG_READ);
        }
    }

    Koi::Network::Manager::get_singleton().cleanup();
    return 0;
}
