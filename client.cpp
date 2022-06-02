/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 01:36:14 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/02 18:35:56 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

const socklen_t Client::address_lenght = sizeof(Client::address);

Client::Client() : address(), request(""), response(), socket_fd(-1), received(0)
{
	// std::cout << "Client constructor called" << std::endl;
}

void	Client::drop_client()
{
	close(socket_fd);
}

Client::~Client()
{
	// std::cout << "Client destructor called" << std::endl;
}