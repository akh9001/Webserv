/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 01:36:14 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/05 22:45:16 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/client.hpp"

socklen_t				Client::address_lenght = sizeof(struct sockaddr);
const struct timeval	Client::timeout = {1, 500000};

Client::Client() : address(), resp_path(), resp_infile(), socket_fd(-1), request()
{
	// std::cout << "Client::Client()" << std::endl;
	last_activity = time(NULL);	
}

Client::Client(const Client  &src) : resp_infile()
{
	*this = src;
}

Client& Client::operator=(const Client& other)
{
	address = other.address;
	resp_path = other.resp_path;
	socket_fd = other.socket_fd;
	last_activity = other.last_activity;
	request = other.request;
	return *this;
}

// bool Client::operator==(int val) const
// {
// 	return (socket_fd == val);
// }

// void	Client::drop_client()
// {
// 	close(socket_fd);
// }

Client::~Client()
{
	// std::cout << "Client destructor called" << std::endl;
}