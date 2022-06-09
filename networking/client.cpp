/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 01:36:14 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/07 06:37:36 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/client.hpp"

socklen_t				Client::address_lenght = sizeof(struct sockaddr);
const struct timeval	Client::timeout = {1, 500000};

Client::Client() : address(), socket_fd(-1), request(), body_inf(), last_activity(NULL)
{
}

Client::Client(const Client  &src) : file()
{
	*this = src;
}

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		this->address = other.address;
		this->socket_fd = other.socket_fd;
		this->request = other.request;
		// this->header_response = other.header_response;
		this->body_inf = other.body_inf;
		this->last_activity = other.last_activity;
	}
	return (*this);
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