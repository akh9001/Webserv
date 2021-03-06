/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:16:46 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/12 09:44:21 by akhalidy         ###   ########.fr       */
/*   Updated: 2022/06/11 11:10:48 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "client.hpp"

// #include <cstdlib>
// #include <sys/_types/_fd_def.h>
// #include <sys/socket.h>
// #include <vector>
class Config;

class Socket 
{
	private:
		int							__socket;
		struct sockaddr_in			__address;
	public:
		static						int	max_socket;
		static fd_set				__master_rd;
		static fd_set				__master_wr;
		static fd_set				__reads;
		static fd_set				__writes;
		Socket();
		void						init(int port, const char * ip_addr);
		void						create_socket();
		void						bind_socket();
		void						listen_socket();
		int							get_socket() const;
		inline static void			accept_connection(int i, std::map<int,  Client> &clients);
		inline static void			read_request(int i, std::map<int,  Client> &clients, Config config); // added config
		inline static void			write_response(int i, std::map<int,  Client> &clients);
		inline static void			remove_client(int i, std::map<int,  Client> &clients, bool rd, bool wr);
		inline static void			reset_read(int i);
		inline static void			reset_write(int i, std::map<int, Client> &clients, bool close);
		inline static void			init_fd_sets_timeout(std::vector<Socket>::const_iterator it, std::vector<Socket>::const_iterator end, struct timeval &timeout);
		inline static void			supervise(std::map<int,  Client> &client_map);
		static void					wait(const std::vector<Socket> &socket_listen, Config config); // config added
		bool						operator==(int val) const;
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