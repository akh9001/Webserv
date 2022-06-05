/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 19:46:37 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/05 05:43:04 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../tmp/request.hpp"
#include "networking.hpp"
#include <string>
#include <sys/_types/_socklen_t.h>

/*
	wait a maximum of 1.5 seconds
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 500000;
	select(max_socket+1, &copy, 0, 0, &timeout);
*/
struct Client
{
	//*static variables :
	static socklen_t			address_lenght;
	const static struct timeval	timeout;
	//*variables :
	struct sockaddr			address;
	char*					resp_path;
	//! istream may cause a problem.
	std::ofstream			resp_outfile;
	int						socket_fd;
	Request					request;
	time_t					last_activity;
	// *constructors :
	Client();
	Client(const Client  &);
	Client& operator=(const Client& other);
	// bool	operator==(int val) const;
	// *destructor :
	// void	drop_client();
	~Client();
};