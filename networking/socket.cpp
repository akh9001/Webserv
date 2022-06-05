/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:21:06 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/05 05:50:19 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/socket.hpp"
#define	TIME_OUT_CLIENT 50

int		Socket::max_socket = 0;
fd_set	Socket::__master_rd = {};
fd_set	Socket::__master_wr = {};
fd_set	Socket::__reads = {};
fd_set	Socket::__writes = {};

Socket::Socket(void){}

void	Socket::init(int port, const char * ip_addr)
{
	memset((char *)&__address, 0, sizeof(__address));
	std::cout << "IP: " << ip_addr << std::endl;
	std::cout << "Port: " << port << std::endl;
	__address.sin_family = AF_INET; // AF_INET for ipv4.
	__address.sin_port = htons(port);
	__address.sin_addr.s_addr = inet_addr(ip_addr);
	//* in_addr_t inet_addr(const char *cp);
	//* convert from *cp (in dotted decimal notation),
	//* to an integer value suitable for use as an Internet address.
}

void		Socket::create_socket()
{
	__socket = socket(PF_INET, SOCK_STREAM, 0);
	if (__socket == -1)
	{
		perror("Socket creation !");
		exit(EXIT_FAILURE);
	}
	else
		max_socket = __socket > max_socket ? __socket : max_socket;
	if (fcntl(__socket, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fncl !");
		exit(EXIT_FAILURE);
	}
}

void	Socket::bind_socket()
{
	int opt = 1;
	setsockopt(__socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
	if (bind(__socket, (sockaddr *) &__address,  sizeof(__address)) == -1)
	{
		perror("bind creation !");
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

// void	Socket::supervise(std::map<int,  Client> &client_map)
// {
// 	std::map<int, Client>::const_iterator	it = client_map.begin();
// 	std::map<int, Client>::const_iterator	end = client_map.end();
// 	time_t	now;
// 	static int i;

// 	now = time(NULL);
// 	if (now == -1)
// 	{
// 		perror("Time error !");
// 		exit(EXIT_FAILURE);
// 	}
// 	for(; it != end; ++it)
// 	{
// 		if (now - it->second.last_activity > 5)
// 		{
// 			FD_CLR(it->first, &__master_rd);
// 			FD_CLR(it->first, &__master_wr);
// 			close(it->first);
// 			client_map.erase(it);
// 		}
// 	}
// }

void	Socket::wait(const std::vector<Socket> socket_listen)
{
	int									ret = 0;
	Client								client;
	std::map<int,  Client>				client_map;
	std::vector<Socket>::const_iterator	it = socket_listen.begin();
	std::vector<Socket>::const_iterator	end = socket_listen.end();

	
	FD_ZERO(&__master_rd);
	FD_ZERO(&__master_wr);
	for(;it < end; it++)
	{
		std::cout << "_sokc " << it->__socket << std::endl;
		FD_SET(it->__socket, &__master_rd);
	}
	while(true)
	{
		__reads = __master_rd;
		__writes = __master_wr;
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		// * std::cout <<"num == >     " <<  __reads.fds_bits[0] << " " << socket_listen[0].__socket << std::endl;
		if ((ret = select(1024, &__reads, &__writes, NULL, &timeout)) == -1)
		{
			perror("select() failed. !");
			exit(EXIT_FAILURE);
		}
		// supervise(client_map);
		// ! why should I start with i = 1 and not 0.
		for (int i = 3; i <= max_socket; i++)
		{
			if(FD_ISSET(i, &__reads))
			{
				if (find(socket_listen.begin(), end, i) != end)
				{
					std::cout << "Waiting for activity... lol ana khadame\n" << std::endl;
					client.socket_fd = accept(i, &client.address, &client.address_lenght);
					std::cout << "Socket : " << client.socket_fd << std::endl;
					client_map[client.socket_fd] = client;
					client_map[client.socket_fd].last_activity = time(NULL);
					FD_SET(client.socket_fd, &__master_rd);
					max_socket = max_socket > client.socket_fd ? max_socket : client.socket_fd;
				}
				else
				{
					char read[1024];
                	int bytes_received = recv(i, read, sizeof(read), 0);
					if(!bytes_received)
					{
						FD_CLR(i, &__master_rd);
						close(i);
						client_map.erase(i);
					}
					if (bytes_received == -1) // I should throw an exception instead of exit.
					{
						perror("recv() failed. !");
						exit(EXIT_FAILURE);
					}
					//1- parse_request
					// Request request(read);
					std::cout << "read " << read << std::endl;
					bool check = client_map[i].request.parse_read(std::string(read));
					if (check)
					{
						FD_CLR(i, &__master_rd);
						FD_SET(i, &__master_wr);
						//2- response_path = get_response()
						
						client_map[i].resp_outfile.open(client_map[i].resp_path);
						// fcntl(fd, F_SETFL, O_NONBLOCK);
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
			else
			{
				time_t	now;
				now = time(NULL);
				// std::cout << "now " << client_map[i].last_activity<< " i = " << i << " max = " << max_socket << std::endl;
				
				if (now - client_map[i].last_activity > TIME_OUT_CLIENT)
				{
					FD_CLR(i, &__master_rd);
					FD_CLR(i, &__master_wr);
					close(i);
					client_map.erase(i);
				}
			}
		}
	}
}

int	Socket::get_socket() const
{
	return (__socket);
}

bool Socket::operator==(int val) const
{
	return (get_socket() == val);
}

Socket::~Socket()
{
	FD_CLR(__socket, &__master_rd);
	FD_SET(__socket, &__master_wr);
	close(__socket);
}