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
#include <network/bound_server.hpp>
#include <network/enums.hpp>

#include <iostream>


int main(int argc, char** argv) {
    std::cout << "Tcp server example started." << std::endl;

    Koi::Network::Error error = Koi::Network::NETWORK_ERROR_OK;
    bool is_running = true;
    Koi::Network::Manager::get_singleton().startup();

    std::string default_hostname = "::1";
    std::string default_port = "8080";

    Koi::Network::BoundServer server;

    if (argc < 3) {
        error = server.open_local_handle(default_hostname.c_str(), default_port.c_str(), 1, Koi::Network::NETWORK_PROTOCOL_STREAM);
    } else {
        error = server.open_local_handle(argv[1], argv[2], 1, Koi::Network::NETWORK_PROTOCOL_STREAM);
    }

    if (error != Koi::Network::NETWORK_ERROR_OK) {
        std::cout << "Unable to open local socket handle. Run with logging enabled for more info." << std::endl;
        return 1;
    } else {
        std::cout << "Server opened." << std::endl;
    }

    Koi::Network::TimeValue timeout { 0, 10000 };
    while(is_running) {
        int select_result = Koi::Network::Manager::get_singleton().select_handles(&timeout);
        if (select_result > 0) {
            int readiness = server.get_readiness();

            while (server.is_new_connection_ready()) {
                Koi::Network::Socket remote = server.accept_new_connection(Koi::Network::NETWORK_SELECT_FLAG_READ);
            }

            if (readiness & Koi::Network::NETWORK_SELECT_FLAG_READ) {
                auto it = server.cbegin_read();
                auto end = server.cend_read();

                while (it != end) {
                    if (Koi::Network::Manager::get_singleton().get_handle_readiness(*it) & Koi::Network::NETWORK_SELECT_FLAG_READ) {
                        char buffer[1024];

                        Koi::Network::SendReceiveResult result = server.receive(*it, buffer, 1024, 0);

                        if (result > 0) {
                            buffer[result] = '\0';
                            std::cout << buffer << std::endl;
                        } else if (result < 0) {
                            //todo:: handle error
                        } else { // result == 0
                            server.remove_remote_handle(*it);//fixme:: can't do this while using iterator - perhaps queue for deletion
                        }
                    }

                    ++it;
                }
            }

        } else if (select_result < 0) {
            is_running = false;
        }
    }

    Koi::Network::Manager::get_singleton().cleanup();
    std::cout << "Tcp server example stopped." << std::endl;
    return 0;
}
