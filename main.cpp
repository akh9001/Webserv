/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 17:31:58 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/11 12:19:14 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/socket.hpp"
#include "Parsing/Config/Config.hpp"
#include <vector>


static std::map<std::string,std::string> statusCodeMessages;

void init_statusCodeMessages()
{

	statusCodeMessages["100"] = "Continue";
	statusCodeMessages["200"] = "OK";
	statusCodeMessages["201"] = "Created";
	statusCodeMessages["202"] = "Accepted";
	statusCodeMessages["203"] = "Non-Authoritative Information";
	statusCodeMessages["204"] = "No Content";
	statusCodeMessages["205"] = "Reset Content";
	statusCodeMessages["206"] = "Partial Content";
	statusCodeMessages["300"] = "Multiple Choices";
	statusCodeMessages["301"] = "Moved Permanently";
	statusCodeMessages["302"] = "Found";
	statusCodeMessages["303"] = "See Other";
	statusCodeMessages["304"] = "Not Modified";
	statusCodeMessages["305"] = "Use Proxy";
	statusCodeMessages["307"] = "Temporary Redirect";
	statusCodeMessages["400"] = "Bad Request";
	statusCodeMessages["401"] = "Unauthorized";
	statusCodeMessages["403"] = "Forbidden";
	statusCodeMessages["404"] = "Not Found";
	statusCodeMessages["405"] = "Method Not Allowed";
	statusCodeMessages["406"] = "Not Acceptable";
	statusCodeMessages["407"] = "Proxy Authentication Required";
	statusCodeMessages["408"] = "Request Time-out";
	statusCodeMessages["409"] = "Conflict";
	statusCodeMessages["410"] = "Gone";
	statusCodeMessages["411"] = "Length Required";
	statusCodeMessages["412"] = "Precondition Failed";
	statusCodeMessages["413"] = "Request Entity Too Large";
	statusCodeMessages["414"] = "Request-URI Too Large";
	statusCodeMessages["415"] = "Unsupported Media Type";
	statusCodeMessages["416"] = "Requested range not satisfiable";
	statusCodeMessages["417"] = "Expectation Failed";
	statusCodeMessages["500"] = "Internal Server Error";
	statusCodeMessages["501"] = "Not Implemented";
	statusCodeMessages["502"] = "Bad Gateway";
	statusCodeMessages["503"] = "Service Unavailable";
	statusCodeMessages["504"] = "Gateway Time-out";
	statusCodeMessages["505"] = "HTTP Version not supported";
}

int	main(int argc, char **argv)
{
	init_statusCodeMessages();
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
	Socket::wait(socket_list, config); // added the config to prototype
}
