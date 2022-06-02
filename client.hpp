/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 19:46:37 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/02 20:56:18 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
	const static socklen_t			address_lenght;
	struct sockaddr		address;
	std::string			header_request;
	// istream may cause a problem.
	char*				resp_path;
	std::ofstream		resp_outfile;
	int					socket_fd;
	int					received;
	// server
	struct timeval		timeout;
	// drop client
	Client();
	void	drop_client();
	~Client();
	
};