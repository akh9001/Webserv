/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 00:20:09 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/12 00:55:58 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

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