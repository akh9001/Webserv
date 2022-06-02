/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:16:46 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/02 18:46:59 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "networking.hpp"

// #include <cstdlib>
// #include <sys/_types/_fd_def.h>
// #include <sys/socket.h>
// #include <vector>

class Socket 
{
	private:
		int					__socket;
		struct sockaddr_in	__address;
		fd_set				__master_rd;
		fd_set				__master_wr;
		fd_set				__reads;
		fd_set				__writes;
	public:
		static	int	max_socket;
		Socket();
		void	init(int port, const char * ip_addr);
		int		create_soket();
		void	bind_socket();
		void	listen_socket();
		void	wait(std::vector<Socket> socket_listen);
		~Socket();
};

//* struct sockaddr_in 
//* { 
//*     __uint8_t         sin_len; 
//*     sa_family_t       sin_family; 
//*     in_port_t         sin_port; 
//*     struct in_addr    sin_addr; 
//*     char              sin_zero[8]; 
//* };