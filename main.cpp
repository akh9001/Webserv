/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 17:31:58 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/28 16:16:02 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/socket.hpp"
#include "Parsing/Config/Config.hpp"
#include <vector>

int	main(int argc, char **argv)
{
	//Config config("default.conf");
	char tmpFolder[] = "./tmp";
	std::string a = "default.conf";
	if (argc == 2)
		a =	argv[1];
	Config config(a);	
	try {
		config.main_read();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		exit(1);
	}
	std::map<int, std::string>  servers = config.getBinders();//  {{8080, "127.0.0.1"}, {8081, "google.com"}}
	std::vector<Socket>	socket_list(servers.size());
	std::map<int, std::string>::iterator it = servers.begin();
	std::map<int, std::string>::iterator end = servers.end();
	
	int									i = 0;
	
	for(; it != end; ++it)
	{
		std::cout << "Binding to " << it->second << " on port " << it->first << std::endl;
		//std::cout << "Creating socket " << i << std::endl;
		socket_list[i].init(it->first, it->second.c_str());
		socket_list[i].create_socket();
		socket_list[i].bind_socket();
		socket_list[i++].listen_socket();
	}
	if (!ws::fileHandler::checkIfExist(tmpFolder))
                mkdir(tmpFolder,0777);
	Socket::wait(socket_list, config); // added the config to prototype
}
