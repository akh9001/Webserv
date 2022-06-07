/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:21:06 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/07 09:18:57 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/socket.hpp"
#include "../dyalek/fileHandler.hpp"
#define	TIME_OUT_CLIENT	50
#define SIZE_BUFFER		1024

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
	if (listen(__socket, 128) == -1)
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
		if ((ret = select(FD_SETSIZE, &__reads, &__writes, NULL, &timeout)) == -1)
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
					client.socket_fd = accept(i, &client.address, &client.address_lenght);
					std::cout << "Socket client : " << client.socket_fd << std::endl;
					client_map[client.socket_fd] = client;
					client_map[client.socket_fd].last_activity = time(NULL);
					FD_SET(client.socket_fd, &__master_rd);
					max_socket = max_socket > client.socket_fd ? max_socket : client.socket_fd;
	// ! fncl
	//TODO http://gnu.ist.utl.pt/software/libc/manual/html_node/Getting-File-Status-Flags.html
	// * http://www.gnu.org/software/libc/manual/html_node/File-Status-Flags.html#:~:text=File%20status%20flags%20are%20used,single%20opening%20of%20the%20file.
	// * I/O Operating Modes, affect how operations such as read and write are done. They are set by open, and can be fetched or changed with fcntl.
	// *  http://www.gnu.org/software/libc/manual/html_node/Operating-Modes.html :: O_NONBLOCK
					if (fcntl(client.socket_fd, F_SETFL, O_NONBLOCK) == -1)
					{
						perror("fcntl !");
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					char read[SIZE_BUFFER+1];
					int bytes_received;
					// ws::Response	res;
					// if (tmp has something)
					// {
					// 	copy tmp to read
					// 	bytes_received = len(tmp)
					// 	clear tmp;
					// }
					// else
                		bytes_received = recv(i, read, sizeof(read), 0);
						read[bytes_received] = '\0';
					if(bytes_received < 0)
					{
						FD_CLR(i, &__master_rd);
						close(i);
						client_map.erase(i);
					}
					// if (bytes_received == -1) // I should throw an exception instead of exit.
					// {
					// 	perror("recv() failed. !");
					// 	exit(EXIT_FAILURE);
					// }
					//1- parse_request
					// Request request(read);
					std::cout << "read " << read << std::endl;
					bool check = client_map[i].request.parse_read(std::string(read));
					if (check)
					{
						FD_CLR(i, &__master_rd);
						FD_SET(i, &__master_wr);
						//2- response = get_response()
						// client_map[i].buffer = getHeaders();
						// client_map[i].body_inf = getbody();
						client_map[i].buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
						client_map[i].body_inf = std::make_pair(std::string("hello_world.html"), false);
						// client_map[i].body_inf = std::make_pair(std::string(), false);
						if (client_map[i].body_inf.first.size() > 0)
							client_map[i].file.open(client_map[i].body_inf.first);
						// fcntl(fd, F_SETFL, O_NONBLOCK);
					}
				}
			}
			else if (FD_ISSET(i, &__writes))
			{
				if (!client_map[i].buffer.empty())
				{
					// if buffer.size < SIZE_BUFFER wash momkin dir problem.
					int bytes_sent = send(i, client_map[i].buffer.c_str(), client_map[i].buffer.size(), 0);
					std::cerr << YELLOW << client_map[i].buffer << " byte send " << bytes_sent  << std::endl << RESET;
					if (bytes_sent == -1)
					{
						perror("send() failed. !");
						// should I close the socket ?
						// should I throw an exception ?
						exit(EXIT_FAILURE);
					}
					
					if (client_map[i].buffer.size() <= bytes_sent)
						client_map[i].buffer.clear();
					else
						client_map[i].buffer = client_map[i].buffer.substr(bytes_sent);
					
				}
				else if (client_map[i].file.is_open())
				{ 	
					char	buff[SIZE_BUFFER];
					
					if (client_map[i].file.eof())
					{
						client_map[i].file.close();
						if (client_map[i].body_inf.second)
							if (remove(client_map[i].body_inf.first.c_str()))
								perror("remove() failed. !");
						FD_CLR(i, &__master_wr);
						FD_SET(i, &__master_rd);
						client_map[i].last_activity= time(NULL);
						continue;
					}
					client_map[i].file.read(buff, SIZE_BUFFER);
					// buff[client_map[i].file.gcount()] = '\0';
					client_map[i].buffer = std::string(buff);
					int bytes_sent = send(i, buff, client_map[i].buffer.size(), 0);
					std::cout << YELLOW << buff << RESET << std::endl;
					// exit(EXIT_FAILURE);
					if (bytes_sent == -1)
					{
						perror("send() failed. !");
						// should I close the socket ?
						// should I throw an exception ?
						exit(EXIT_FAILURE);
					}
					if (client_map[i].buffer.size() > bytes_sent)
						client_map[i].buffer = client_map[i].buffer.substr(bytes_sent);
					else
						client_map[i].buffer.clear();
				}
				// if (client_map[i].buffer.empty() && client_map[i].body_inf.second))	
					// std::string	read;

					// client_map[i].file.read(read, SIZE_BUFFER);
					// int bytes_sent = send(i, read.c_str(), read.size(), 0);					
				// std::string		buffer;
				
				
				// buffer = ws::fileHandler::readFile(client_map[i].resp_infile);
				// if (!client_map[i].resp_infile)
				// {
				// 	// client_map[i].resp_infile.close();
				// 	FD_CLR(i, &__master_wr);
				// 	FD_SET(i, &__master_rd);
				// 	client_map[i].last_activity= time(NULL);
				// 	// unlink(client_map[i].resp_path);// std:;remove
				// 	continue;
				// }
				// int sent = send(i, buffer.c_str(), buffer.size(), 0);
				
				// long pos = client_map[i].resp_infile.tellg();
				// if (sent != (int)buffer.size())
				// 	client_map[i].resp_infile.seekg(pos - sent);
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