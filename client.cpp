/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 01:36:14 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/03 02:40:31 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

socklen_t				Client::address_lenght = sizeof(Client::address);
const struct timeval	Client::timeout = {1, 500000};

Client::Client() : address(), resp_path(), resp_outfile(), socket_fd(-1)
{
}

Client::Client(const Client  &src) : resp_outfile()
{
	*this = src;
}

Client& Client::operator=(const Client& other)
{
	address = other.address;
	resp_path = other.resp_path;
	socket_fd = other.socket_fd;
	return *this;
}

void	Client::drop_client()
{
	close(socket_fd);
}

Client::~Client()
{
	// std::cout << "Client destructor called" << std::endl;
}