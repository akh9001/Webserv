/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 17:31:58 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/10 03:20:31 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/socket.hpp"
#include <vector>



int	main(int argc, char **argv)
{
	(void)argv;
	signal(SIGPIPE, SIG_IGN);
	std::map<int, const char*>  servers ;//  {{8080, "127.0.0.1"}, {8081, "google.com"}};
	servers.insert(std::make_pair(2025, "127.0.0.1"));
	servers.insert(std::make_pair(8081, "127.0.0.1"));
	
	std::vector<Socket>	socket_list(servers.size());
	std::map<int, const char*>::iterator it = servers.begin();
	std::map<int, const char*>::iterator end = servers.end();
	int									i = 0;
	
	for(; it != end; ++it)
	{
		std::cout << "Creating socket " << i << std::endl;
		socket_list[i].init(it->first, it->second);
		socket_list[i].create_socket();
		socket_list[i].bind_socket();
		socket_list[i++].listen_socket();
	}
	Socket::wait(socket_list);
}