/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:16:46 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/09 10:41:50 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client.hpp"

// #include <cstdlib>
// #include <sys/_types/_fd_def.h>
// #include <sys/socket.h>
// #include <vector>

class Socket 
{
	private:
		int						__socket;
		struct sockaddr_in		__address;
	public:
		static	int	max_socket;
		static fd_set			__master_rd;
		static fd_set			__master_wr;
		static fd_set			__reads;
		static fd_set			__writes;
		Socket();
		void				init(int port, const char * ip_addr);
		void				create_socket();
		void				bind_socket();
		void				listen_socket();
		int					get_socket() const;
		static bool			accept_connection(int i, std::map<int,  Client> &clients);
		static bool			read_request(int i, std::map<int,  Client> &clients);
		static bool			write_response(int i, std::map<int,  Client> &clients);
		static void			remove_client(int i, std::map<int,  Client> &clients, bool rd, bool wr);
		static void			reset_read(int i);
		static void			reset_write(int i);
		static void			init_fd_sets_timeout(std::vector<Socket>::const_iterator it, std::vector<Socket>::const_iterator end, struct timeval &timeout);
		// static	void		supervise(std::map<int,  Client> &client_map);
		static void			wait(const std::vector<Socket> &socket_listen);
		bool				operator==(int val) const;
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