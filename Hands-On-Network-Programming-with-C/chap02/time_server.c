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
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif


#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif


#include <stdio.h>
#include <string.h>
#include <time.h>


int main() {


#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif


    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);
    // char address_buffer2[100];
    // char address_buffer3[100];
	// socklen_t hints_len = sizeof(bind_address->ai_addr);
    // getnameinfo(bind_address->ai_addr, hints_len, address_buffer2, sizeof(address_buffer2), address_buffer3, sizeof(address_buffer3), NI_NUMERICHOST);
    // printf("Server add : %s\n", address_buffer2);
    // printf("Server add : %s\n", address_buffer3);

//*1-1 AF_INET : specifies that we are looking for an IPv4 address. (AF_INET6 for IPv6)
//*1-2 ai_socktype = SOCK_STREAM : This indicates that we're going to be using TCP. (SOCK_DGRAM for UDP)
// ?  Stream sockets : connection-oriented sockets, use continuous streams of characters, and communicate using the reliable Transmission Control Protocol (TCP).
// ?  Datagram sockets expect a unit of data; they read the entire message at once. These message-oriented sockets use the UNIX datagram protocol (UTP).
//*1-3 ai_flags = AI_PASSIVE : Makes getaddrinfo() bind it to the wildcard address (0.0.0.0). (The wildcard is a special local IP address. It usually means "any").
//*1-3 (That is, we are asking getaddrinfo() to set up the address, so we listen on any available network interface.)
//? https://stackoverflow.com/questions/11931175/what-does-wildcard-address-in-inetsocketaddress-mean
//# The getaddrinfo() function has many uses, but for our purpose, it generates an address that's suitable for bind(). 
//#	To make it generate this, we must pass in the first parameter as NULL and have the AI_PASSIVE flag set in hints.ai_flags. 
// 1-4 The second parameter to getaddrinfo() is the port we listen for connections on. 
// 1-4 A standard HTTP server would use port 80. However, only privileged users on Unix-like operating systems can bind to ports 0 through 1023.
// 1-4 Keep in mind that only one program can bind to a particular port at a time. If you try to use a port that is already in use, then the call to bind() fails. 

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
// 2-1 socket() : generate the socket. It takes 3 parameters: the socket family(IPv4/6), the socket type(datagram / streaming), and the socket protocol(TCP/UDP).
	//# TCP stands for Transmission Control Protocol a communications standard that enables application programs and computing devices to exchange messages over a network.
	// # It is designed to send packets across the internet and ensure the successful delivery of data and messages over networks.
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Binding socket to local address...\n");

// # Binding socket associates a socket with a particular local IP address and port number.
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) 
	{
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);


    printf("Listening...\n");
// * On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error.
// * The second argument to listen(), which is 10 in this case, tells listen() how many connections it is allowed to queue up.
// * If 10 connections become queued up, then the operating system will reject new connections until we remove one from the existing queue.
    if (listen(socket_listen, 10) < 0)
	{
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client)) {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    printf("%s\n", address_buffer);


    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);
    //printf("%.*s", bytes_received, request);


    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));


    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);


#if defined(_WIN32)
    WSACleanup();
#endif


    printf("Finished.\n");

    return 0;
}
