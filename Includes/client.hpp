/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 19:46:37 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/30 15:09:53 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "../Parsing/Request/Request.hpp"
#include <string>
class Request;

struct Client
{
	//*static variables :
	static socklen_t					address_lenght;
	const static struct timeval			timeout;
	//*variables :
	struct sockaddr						address;
	int									socket_fd;
	Request								request;
	std::pair<std::string, bool>		body_inf;
	std::ifstream						file;
	std::string							buffer;
	time_t								last_activity;
	bool								close_cnx;
	// *constructors :
	Client();
	Client(const Client  &);
	Client& operator=(const Client& other);
	// bool	operator==(int val) const;
	// *destructor :
	// void	drop_client();
	~Client();
};
#endif