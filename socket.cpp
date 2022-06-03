/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:21:06 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/03 02:43:52 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include "client.hpp"
#include <sys/socket.h>
#include <vector>


	int	Socket::max_socket = 0;
	Socket::Socket(void){}
	
	void	Socket::init(int port, const char *ip_addr)
	{
		memset((char *)&__address, 0, sizeof(__address));
		__address.sin_family = AF_INET; // AF_INET for ipv4.
		__address.sin_port = htons(port);
		__address.sin_addr.s_addr = inet_addr(ip_addr);
		//* in_addr_t inet_addr(const char *cp);
		//* convert from *cp (in dotted decimal notation),
		//* to an integer value suitable for use as an Internet address.
	}
	
	int		Socket::create_soket()
	{
		__socket = socket(AF_INET, SOCK_STREAM, 0);
		if (__socket == -1)
		{
			perror("Socket creation !");
			exit(EXIT_FAILURE);
		}
		else
			max_socket = __socket > max_socket ? __socket : max_socket;
		return __socket;
	}
	
	void	Socket::bind_socket()
	{
		if (bind(__socket, (struct sockaddr *) &__address,  sizeof(__address) == -1))
		{
			perror("Socket creation !");
			exit(EXIT_FAILURE);
		}
	}
	
	void	Socket::listen_socket()
	{
		if (listen(__socket, 10) == -1)
		{
			perror("Listening... !");
			exit(EXIT_FAILURE);
		}
	}
	
	void	Socket::wait(const std::vector<Socket> socket_listen)
	{
		Client								client;
		std::map<int,  Client>				client_map;
		std::vector<Socket>::const_iterator	it = socket_listen.begin();
		std::vector<Socket>::const_iterator	end = socket_listen.end();
		int									server_max_socket = (end - 1)->__socket;
		int									socket_client;

		FD_ZERO(&__master_rd);
		FD_ZERO(&__master_wr);
		for(;it < end; it++)
			FD_SET(it->__socket, &__master_rd);
		while(true)
		{
			__reads = __master_rd;
			__writes = __master_wr;
			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 50;
			int ret;
			if ((ret = select(max_socket + 1, &__reads, &__writes, NULL, &timeout)) == -1)
			{
				perror("select() failed. !");
				exit(EXIT_FAILURE);
			}
			if(!ret)
				continue;
			// ! why should I start with i = 1 and not 0.
			for (int i = 1; i <= max_socket; i++)
			{
				if(FD_ISSET(i, &__reads))
				{
					if (find(socket_listen.begin(), end, i) != end)
					{
						client.socket_fd = accept(i, &client.address, &client.address_lenght);
						client_map[client.socket_fd] = client;
						FD_SET(client.socket_fd, &__master_rd);
						max_socket = max_socket > client.socket_fd ? max_socket : client.socket_fd;
					}
					else
					{
						char read[1024];
                    	int bytes_received = recv(i, read, sizeof(read), 0);
						//1- parse_request
						// Request request(read);
						if (true)
						{
							FD_CLR(i, &__master_rd);
							FD_SET(i, &__master_wr);
							//2- response_path = get_response()
							client_map[i].resp_outfile.open(client_map[i].resp_path);
						}
					}
				}
				else if (FD_ISSET(i, &__writes))
				{
					//open file , read
					char	buffer[1024];
					int	r  = read(i, buffer, sizeof(buffer));
					if (!r)
					{
						client_map[i].resp_outfile.close();
						FD_CLR(i, &__master_wr);
						unlink(client_map[i].resp_path);
					}
					int sent = send(i, buffer, sizeof(buffer), 0);
					long pos = client_map[i].resp_outfile.tellp();
					if (sent != r)
						client_map[i].resp_outfile.seekp(pos - sent);
				}
			}
		}
	}
	
	Socket::~Socket()
	{
	}