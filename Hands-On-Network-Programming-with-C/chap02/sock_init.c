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

// #if defined(_WIN32)
// #ifndef _WIN32_WINNT
// #define _WIN32_WINNT 0x0600
// #endif
// #include <winsock2.h>
// #include <ws2tcpip.h>
// #pragma comment(lib, "ws2_32.lib")

// #else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

// #endif

#include <stdio.h>

int main() {

// #if defined(_WIN32)
//     WSADATA d;
//     if (WSAStartup(MAKEWORD(2, 2), &d)) {
//         fprintf(stderr, "Failed to initialize.\n");
//         return 1;
//     }
// #endif

    printf("Ready to use socket API.\n");

// #if defined(_WIN32)
//     WSACleanup();
// #endif

    return 0;
}
//# Socket : (Sockets were used for inter-process communication)
// * A socket is one endpoint of a communication link between systems.
// * Your application sends and receives all of its network data through a socket.
// * There is two diffrents versions of sockets :
// 1-  Berkeley socket :
	// Unix sockets.
// 2- Winsock :
	// Windows' socket :
	// Winsock requires initialization before use. It also requires that a cleanup
	// function is called when we are finished.
// # Types of socket
// ? Sockets come in two basic types—connection-oriented and connectionless.
// * In a connectionless protocol, such as UDP, each data packet is addressed individually. 
	// 1- From the protocol's perspective, each data packet is completely independent and unrelated to any packets coming before or after it.
	// 2- A good analogy for UDP is postcards. When you send a postcard, there is no guarantee that it will arrive. There is also no way to know if it did arrive.
	//* 3- If you send many postcards at once, there is no way to predict what order they will arrive in. It is entirely possible that the first postcard you send gets delayed and arrives weeks after the last postcard was sent.
	// * 4- it is possible that a single packet may arrive twice!
		// ! => UDP NOT RELIABLE (trusted).
	// * UDP is also commonly used in real-time applications, such as audio streaming, video streaming, and multiplayer video games.
	// * In real-time applications, there is often no reason to retry sending dropped packets, so TCP's guarantees are unnecessary.
// * TCP guarantees that data arrives in the same order it is sent.
	//	1- It prevents duplicate data from arriving twice.
	//	2- It retries sending missing data.
	//*	3- the packets sent are numbered.
	//* 4- an aknowledgment is send for every packet received.
		// ? For these reasons, TCP is used by many protocols.
			// 1-1 HTTP (for severing web pages).
			// 1-2 FTP (for transferring files).
			// 1-3 SSH (for remote administration).
			// 1-3 SMTP (for delivering email).

// # The socket APIs provide many functions for use in network programming. 
// Here are the common socket functions:
// * socket()	: creates and initializes a new socket.
// * bind()		: associates a socket with a particular local IP address and port number.
// * listen()	: is used on the SERVER to cause a TCP socket to listen for new connections.
// * connect()	: is used on the CLIENT to set the remote address and port. In the case of TCP, it also establishes a connection.
// * accept() 	: is used on the server to create a new socket for an incoming TCP connection.
// * send() and recv() : are used to send and receive data with a socket.
// ? sendto() and recvfrom() : are used to send and receive data from sockets without a bound remote address.
// * close() (Berkeley sockets) and closesocket() (Winsock sockets) : are used to close a socket. In the case of TCP, this also terminates the connection.
// ? shutdown() : is used to close one side of a TCP connection. It is useful to ensure an orderly connection teardown.
// * select() 	: is used to wait for an event on one or more sockets.
// ? getnameinfo() and getaddrinfo() provide a protocol-independent manner of working with hostnames and addresses.
// * setsockopt(): is used to change some socket options.
// * fcntl() (Berkeley sockets) and ioctlsocket() (Winsock sockets) are also used to get and set some socket options.

// # client-server paradigm (a model of something, or a very clear and typical example of something).
	// *1- In this paradigm, a server listens for new connections at a published address.
	// *2- The client, knowing the server's address, is the one to establish the connection initially.
	// * 3- Once the connection is established, the client and the server can both send and receive data.
	// * 4- This can continue until either the client or the server terminates the connection.
/*
	*	A traditional client-server model usually implies different behaviors for the client and server.
	*	The way web browsing works, for example, is that the server resides at a known address, waiting for connections.
	*	A client (web browser) establishes a connection and sends a request that includes which web page or resource it wants to download.
	*	The server then checks that it knows what to do with this request and responds appropriately (by sending the web page).
*/

// # TCP program flow :
// Client :
	// *1-1 The client should know the server adress.(input by the user)
	// *1-2 The client then creates a socket using a call to socket().
	// *1-3 The client establishes the new TCP connection by calling connect().
	// *1-4 At this point, the client can freely exchange data using send() and recv().
// Server :
	// *2-1 The server listens for connections at a particular port number on a particular interface.
	// *2-? The program must first initialize a struct addrinfo structure with the proper listening IP address and port number.
	// ? The getaddrinfo() function is helpful so that you can do this in an IPv4/IPv6 independent way.
	// *2-2 The server then creates the socket with a call to socket().
	// *2-3 The socket must be bound to the listening IP address and port. Using bind().
	// *2-4 The server program then calls listen(), to lisen for new cnx.
	// *2-5 The server can then call accept(). When the new connection has been established, accept() returns a new socket.
	// *2-7 This new socket can be used to exchange data with the client using send() and recv().
	// *2-8 Meanwhile, the first socket remains listening for new connections, and repeated calls to accept() allow the server to handle multiple clients.

// ! An endpoint :
// * An endpoint is any device that is physically an end point on a network. (is any device that connects to a computer network).
// * Laptops, desktops, mobile phones, tablets, servers, and virtual environments can all be considered endpoints.