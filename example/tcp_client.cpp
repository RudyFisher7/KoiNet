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


#include <network/internal.hpp>


#include <string>
#include <cstring>
#include <thread>

#include <cstdio>
#include <ctime>


#if defined(_WIN32)
#include <conio.h>
#endif


namespace KoiNet = Koi::Network;


int main(int argc, char** argv) {
    KoiNet::Internal::startup();

    std::string default_hostname = "127.0.0.1";
    std::string default_port = "8080";

    printf("configuring remote address... ");
    KoiNet::AddressInfo hints = KoiNet::Internal::get_clean_address_info();
    hints.ai_socktype = SOCK_STREAM;
    KoiNet::AddressInfo* peer_address = nullptr;

    // getaddrinfo() will decide which ai_family to use based on the hostname.
    if (argc < 3) {
        if (
                KoiNet::Internal::get_address_info(
                        default_hostname.c_str(),
                        default_port.c_str(),
                        &hints, &peer_address
                )
        ) {
            return 1;
        }
    } else if (
            KoiNet::Internal::get_address_info(
                    argv[1],
                    argv[2],
                    &hints,
                    &peer_address
            )
    ) {
        return 1;
    }

    char addr_buffer[100];
    char service_buffer[100];
    KoiNet::Internal::get_name_info(
            peer_address->ai_addr,
            peer_address->ai_addrlen,
            addr_buffer,
            sizeof(addr_buffer),
            service_buffer,
            sizeof(service_buffer),
            NI_NUMERICHOST
    );

    printf("%s:%s... done.\n", addr_buffer, service_buffer);

    KoiNet::Socket socket_peer;
    socket_peer = KoiNet::Internal::create_handle(*peer_address);

    if (!KoiNet::Internal::is_socket_valid(socket_peer)) {
        return 2;
    }

    printf("connecting... ");

    // associates a socket with a remote address
    if (
            KoiNet::Internal::bind_remotely(
                    socket_peer,
                    peer_address->ai_addr,
                    peer_address->ai_addrlen
            )
    ) {
        printf("failed.");
        return 3;
    } else {
        printf("done.\n");
    }

    KoiNet::Internal::free_address_info(peer_address);

    while (true) {
        KoiNet::SocketSet master_set {};
        KoiNet::Internal::clean_socket_set(&master_set);
        KoiNet::Internal::set_socket_in_set(socket_peer, &master_set);

#if !defined(_WIN32)
        KoiNet::Internal::set_socket_in_set(fileno(stdin), &master_set); // unix systems can use select() to monitor stdin too
#endif

        KoiNet::SocketSet read_set = master_set;
        KoiNet::SocketSet write_set = master_set;
        KoiNet::SocketSet exception_set = master_set;

        KoiNet::TimeValue timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        if (KoiNet::Internal::select_handles(socket_peer + 1, &read_set, &write_set, &exception_set, &timeout) < 0) {
            return 4;
        }

        if (KoiNet::Internal::is_socket_ready_in_set(socket_peer, &read_set)) {
            char read[4096];
            KoiNet::SendReceiveResult bytes_read = KoiNet::Internal::receive_over_stream(socket_peer, read, 4096, 0);
            if (bytes_read < 1) {
                printf("connection closed by peer.\n");
                break;
            } else {
                // note: 'read' char array will not be null terminated, %.*s prints a string of specified length
                printf("received (%d bytes): %.*s", bytes_read, bytes_read, read);
            }
        }

#if defined(_WIN32)
        if (_kbhit()) {
#else
        if (KoiNet::Internal::is_socket_ready_in_set(fileno(stdin), &read_set)) {
#endif
            char read[4096];
            if (!fgets(read, 4096, stdin)) {
                break;
            } else {
                printf("sending %s... ", read);
                KoiNet::SendReceiveResult bytes_sent = send(socket_peer, read, (KoiNet::BufferSize)strlen(read), 0);
                printf("sent %d bytes.\n", bytes_sent);
            }
        }
    }

    KoiNet::Internal::close_handle(socket_peer);

    std::this_thread::sleep_for(std::chrono::seconds(16));

    Koi::Network::Internal::cleanup();
    return 0;
}