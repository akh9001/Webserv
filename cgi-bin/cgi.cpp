/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:00 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/15 10:52:29 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cgi.hpp"

void	CGI::init_env(const Server &server, const Request &request)
{
	// _meta_variables["CONTENT_LENGTH"] = "0";
	// _meta_variables["CONTENT_TYPE"] = "text/html";
	// _meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	// _meta_variables["HTTP_ACCEPT"] = "*/*";
	
	_meta_variables["DOCUMENT_ROOT"] = server.getRoot();
	_meta_variables["HTTP_COOKIE"] = ""; //? I should investigate about this *//
	_meta_variables["HTTP_HOST"] = server.getIp();
	_meta_variables["HTTP_REFERER"] = "";  //? I should investigate about this *//
	//! _meta_variables["HTTP_USER_AGENT"] = server.getUserAgent();
	_meta_variables["HTTPS"] = "off";
	_meta_variables["PATH"] = getenv("PATH");
	_meta_variables["QUERY_STRING"] =
	_meta_variables["REMOTE_ADDR"] = server.getIp();
	_meta_variables["REMOTE_HOST"] =
	_meta_variables["REMOTE_PORT"] =
	_meta_variables["REMOTE_USER"] =
	_meta_variables["REQUEST_METHOD"] = request.getMethod();
	_meta_variables["REQUEST_URI"] =
	// _meta_variables["SCRIPT_FILENAME"] =
	_meta_variables["SCRIPT_NAME"] =
	// _meta_variables["SERVER_ADMIN"] =
	_meta_variables["SERVER_NAME"] = server.getIp();
	_meta_variables["SERVER_PORT"] = server.getPort();
	_meta_variables["SERVER_SOFTWARE"] = "Webserv";
}