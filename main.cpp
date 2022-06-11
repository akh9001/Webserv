/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 17:31:58 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/05 02:54:58 by akhalidy         ###   ########.fr       */
=======
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 17:31:58 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/10 12:10:36 by mokhames         ###   ########.fr       */
>>>>>>> akhalidy
/*                                                                            */
/* ************************************************************************** */

#include "Includes/socket.hpp"
<<<<<<< HEAD
=======
#include "Parsing/Config/Config.hpp"
>>>>>>> akhalidy
#include <vector>



int	main(int argc, char **argv)
{
<<<<<<< HEAD
	(void)argv;
	if (argc != 2)
	{
		std::cout << "Usage: ./client configfile" << std::endl;
		return (1);
	}
	std::map<int, const char*>  servers ;//  {{8080, "127.0.0.1"}, {8081, "google.com"}};
	servers.insert(std::make_pair(2025, "127.0.0.1"));
	servers.insert(std::make_pair(8081, "127.0.0.1"));
	
	std::vector<Socket>	socket_list(servers.size());
	std::map<int, const char*>::iterator it = servers.begin();
	std::map<int, const char*>::iterator end = servers.end();
=======
	//Config config("default.conf");
	std::string a = "default.conf";
	if (argc == 2)
		a =	argv[1];
	Config config(a);	
	try {
		config.main_read();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	std::map<int, std::string>  servers = config.getBinders();//  {{8080, "127.0.0.1"}, {8081, "google.com"}}
	std::vector<Socket>	socket_list(servers.size());
	std::map<int, std::string>::iterator it = servers.begin();
	std::map<int, std::string>::iterator end = servers.end();
	
>>>>>>> akhalidy
	int									i = 0;
	
	for(; it != end; ++it)
	{
<<<<<<< HEAD
		std::cout << "Creating socket " << i << std::endl;
		socket_list[i].init(it->first, it->second);
=======
		std::cout << "Binding to " << it->second << " on port " << it->first << std::endl;
		//std::cout << "Creating socket " << i << std::endl;
		socket_list[i].init(it->first, it->second.c_str());
>>>>>>> akhalidy
		socket_list[i].create_socket();
		socket_list[i].bind_socket();
		socket_list[i++].listen_socket();
	}
<<<<<<< HEAD
	Socket::wait(socket_list);
=======
	Socket::wait(socket_list, config); // added the config to prototype
>>>>>>> akhalidy
}