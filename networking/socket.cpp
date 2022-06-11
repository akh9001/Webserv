/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:21:06 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/11 12:21:05 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/socket.hpp"
#include "../Parsing/Config/Config.hpp"
#include "../Response/Response.hpp"
#include <exception>
#include <string>
#include <unistd.h>
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

void	Socket::supervise(std::map<int,  Client> &client_map)
{
	std::map<int, Client>::const_iterator	it = client_map.begin();
	std::map<int, Client>::const_iterator	end = client_map.end();
	time_t	now;

	now = time(NULL);
	if (now == -1)
	{
		perror("Time error !");
		return ;
	}
	for(; it != end; ++it)
	{
		if (now - it->second.last_activity > TIME_OUT_CLIENT)
		{
			FD_CLR(it->first, &__master_rd);
			FD_CLR(it->first, &__master_wr);
			close(it->first);
			client_map.erase(it);
			break;
		}
	}
}

// ! fncl
//TODO http://gnu.ist.utl.pt/software/libc/manual/html_node/Getting-File-Status-Flags.html
// * http://www.gnu.org/software/libc/manual/html_node/File-Status-Flags.html#:~:text=File%20status%20flags%20are%20used,single%20opening%20of%20the%20file.
// * I/O Operating Modes, affect how operations such as read and write are done. They are set by open, and can be fetched or changed with fcntl.
// *  http://www.gnu.org/software/libc/manual/html_node/Operating-Modes.html :: O_NONBLOCK

bool	Socket::accept_connection(int i, std::map<int,  Client> &clients)
{
	Client	client;

	client.socket_fd = accept(i, &client.address, &client.address_lenght);
	if (client.socket_fd < 0 || client.socket_fd >= FD_SETSIZE)
		return false;
	//! To remove afterwards :
	// std::cout << RED << "Socket client : " << client.socket_fd << RESET << std::endl;
	clients[client.socket_fd] = client;
	clients[client.socket_fd].last_activity = time(NULL);
	FD_SET(client.socket_fd, &__master_rd);
	max_socket = max_socket > client.socket_fd ? max_socket : client.socket_fd;
	if (fcntl(client.socket_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl !");
		return false;
	}
	return true;
}

void	Socket::remove_client(int i, std::map<int,  Client> &clients, bool rd, bool wr)
{
	if (rd)
		FD_CLR(i, &__master_rd);
	if (wr)
		FD_CLR(i, &__master_wr);
	close(i);
	clients.erase(i);	
}

void	Socket::reset_read(int i)
{
	FD_CLR(i, &__master_rd);
	FD_SET(i, &__master_wr);
}

void	Socket::reset_write(int i, std::map<int,  Client> &clients, bool close)
{
	//todo hna 5assni checki 3la keep alive
	if (close)
		remove_client(i, clients, false, true);
	else
	{
		FD_CLR(i, &__master_wr);
		FD_SET(i, &__master_rd);
		clients[i].request.clear();
		clients[i].last_activity = time(NULL);
	}
}

bool	Socket::read_request(int i, std::map<int, Client> &clients, Config config)  // config added
{
	char read[SIZE_BUFFER + 1];
	std::string	status = "-1";
	int bytes_received;
	bool check;

	bytes_received = recv(i, read, sizeof(read), 0);
	if (bytes_received < 0)
	{
		// * remove_client(int i, std::map<int,  Client> &clients, bool rd, bool wr)
		remove_client(i, clients, true, false);
		return false;
	}
	read[bytes_received] = '\0';
	//! I should remove the following line it afterwards.
	// std::cout << "read " << read << std::endl;
	try
	{
		check = clients[i].request.parseChunks(std::string(read, bytes_received), config); // added the config
	}
	catch (const char *flag)
	{
		status = std::string(flag);
	}
	// check = true;
	if (check)
	{
		reset_read(i);
		//! clients[i].close_cnx = clients[i].request.getHeaderMap()["Connection"] == "keep-alive" ? false : true;
		clients[i].close_cnx = true;
		//2- response = get_response()
		ws::Response response;
		// std::string getHeaders(ws::Req req , string statusCode)
		// client_map[i].buffer = getHeaders(clients[i].request, status);
		// client_map[i].body_inf = getbody();
		clients[i].buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 54\r\nConnection: close\r\n\r\n";
		clients[i].body_inf = std::make_pair(std::string("hello_world.html"), false);
		if (clients[i].body_inf.first.size() > 0)
			clients[i].file.open(clients[i].body_inf.first);
	return true;
	}
	return true;
}

bool	Socket::write_response(int i, std::map<int,  Client> &clients)
{
	int		bytes_sent;
	int		bytes_read;
	char	buff[SIZE_BUFFER];
	
	if (!clients[i].buffer.empty())
	{
		// std::cout << "size >> " << clients[i].buffer.length() << std::endl;
		bytes_sent = send(i, clients[i].buffer.c_str(), clients[i].buffer.length(), 0);
	}
	else if (clients[i].file.is_open())
	{ 	
		
		if (clients[i].file.eof())
		{
			clients[i].file.close();
			if (clients[i].body_inf.second)
				if (remove(clients[i].body_inf.first.c_str()))
					perror("remove() failed. !");
			reset_write(i, clients, clients[i].close_cnx);
			return false;
		}
		//! should I check if the read is correctly done ?
		clients[i].file.read(buff, SIZE_BUFFER);
		bytes_read = clients[i].file.gcount();
		clients[i].buffer = std::string(buff, bytes_read);
		bytes_sent = send(i, buff, clients[i].buffer.size(), 0);
	}
	else
	{
		reset_write(i, clients, clients[i].close_cnx);
		return false;
	}
	// std::cout << YELLOW << clients[i].buffer << RESET << " byte send " << bytes_sent << " Bytes read : " << bytes_read << std::endl;
	if (bytes_sent <= 0)
	{
		perror("send() failed. !");
		remove_client(i, clients, false, true);
		return false;
	}
	if (clients[i].buffer.size() > bytes_sent)
		clients[i].buffer = clients[i].buffer.substr(bytes_sent);
	else
		clients[i].buffer.clear();
	return true;
}

void	Socket::init_fd_sets_timeout(std::vector<Socket>::const_iterator it, std::vector<Socket>::const_iterator end, struct timeval &timeout)
{
	signal(SIGPIPE, SIG_IGN);
	FD_ZERO(&__master_rd);
	FD_ZERO(&__master_wr);
	timeout.tv_sec	= 1;
	timeout.tv_usec = 0;
	for(;it < end; it++)
	{
		std::cout << "_sock " << it->__socket << std::endl;
		FD_SET(it->__socket, &__master_rd);
	}
}

void	Socket::wait(const std::vector<Socket> &socket_listen, Config config)
{
	int									ret;
	struct timeval						timeout;
	std::map<int,  Client>				client_map;
	std::vector<Socket>::const_iterator	it = socket_listen.begin();
	std::vector<Socket>::const_iterator	end = socket_listen.end();

	init_fd_sets_timeout(it, end, timeout);
	while(true)
	{
		__reads = __master_rd;
		__writes = __master_wr;
	
		if ((ret = select(FD_SETSIZE, &__reads, &__writes, NULL, &timeout)) == -1)
		{
			perror("select() failed. !");
			continue;
		}
		supervise(client_map);
		//? check timeout 
		for (int i = 3; i <= max_socket; i++)
		{
			if(FD_ISSET(i, &__reads))
			{
				if (find(it, end, i) != end)
				{
					if (!accept_connection(i, client_map))
						continue;
				}
				else
					read_request(i, client_map, config);
			}
			else if (FD_ISSET(i, &__writes))
			{
				if (write_response(i, client_map))
					continue;
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