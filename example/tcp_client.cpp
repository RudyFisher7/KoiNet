/*
 * MIT License
 *
 * Copyright (c) 2018 Lewis Van Winkle
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else //Unix
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif


#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
typedef int SOCKET;
#endif


#if defined(_WIN32)
#include <conio.h>
#endif


#include <string>
#include <thread>

#include <stdio.h>
#include <time.h>


int main(int argc, char** argv) {
#if defined(_WIN32)
    WSAData d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed 1");
        return 1;
    }
#endif

    std::string default_hostname = "example.com";// "127.0.0.1";
    std::string default_port = "80";// "8080";

    printf("configuring remote address... ");
    addrinfo hints {};
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* peer_address = nullptr;

    // getaddrinfo() will decide which ai_family to use based on the hostname.
    if (argc < 3) {
        if (getaddrinfo(default_hostname.c_str(), default_port.c_str(), &hints, &peer_address)) {
            return 1;
        }
    } else if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
        return 1;
    }

    char addr_buffer[100];
    char service_buffer[100];
    getnameinfo(
            peer_address->ai_addr,
            peer_address->ai_addrlen,
            addr_buffer,
            sizeof(addr_buffer),
            service_buffer,
            sizeof(service_buffer),
            NI_NUMERICHOST
    );

    printf("%s:%s... done.\n", addr_buffer, service_buffer);

    SOCKET socket_peer;
    socket_peer = socket(
            peer_address->ai_family,
            peer_address->ai_socktype,
            peer_address->ai_protocol
    );

    if (!ISVALIDSOCKET(socket_peer)) {
        return 2;
    }

    printf("connecting... ");

    // associates a socket with a remote address
    // note: bind() associates a socket with a local address
    if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)) {
        printf("failed.");
        return 3;
    } else {
        printf("done.\n");
    }

    freeaddrinfo(peer_address);

    while (true) {
        fd_set reads {};
        FD_ZERO(&reads);
        FD_SET(socket_peer, &reads);

#if !defined(_WIN32)
        FD_SET(fileno(stdin), &reads); // unix systems can use select() to monitor stdin too
#endif

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        if (select(socket_peer + 1, &reads, 0, 0, &timeout) < 0) {
            return 4;
        }

        if (FD_ISSET(socket_peer, &reads)) {
            char read[4096];
            int bytes_read = recv(socket_peer, read, 4096, 0);
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
        if (FD_ISSET(fileno(stdin), &reads)) {
#endif
            char read[4096];
            if (!fgets(read, 4096, stdin)) {
                break;
            } else {
                printf("sending %s... ", read);
                int bytes_sent = send(socket_peer, read, strlen(read), 0);
                printf("sent %d bytes.\n", bytes_sent);
            }
        }
    }

    CLOSESOCKET(socket_peer);

        std::this_thread::sleep_for(std::chrono::seconds(4));

#if defined(_WIN32)
    WSACleanup();
#endif
    return 0;
}