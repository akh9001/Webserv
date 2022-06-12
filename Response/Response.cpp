/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:59 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/12 04:37:55 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <ctime> 
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

namespace ws {

	Response::Response():buildResponseTry(0),response_is_tmp(false)
	{
		
	}
	Response::~Response(){};

	std::string Response::getHeaders(Request &request, Config &config, std::string statusCode)
	{
		
		
		return "";
	}

	std::pair<std::string, bool> Response::getbody()
	{
		return std::pair<std::string,bool>(this->bodyPath,this->response_is_tmp);
	}
}