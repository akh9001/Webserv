/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:59 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/13 01:54:33 by laafilal         ###   ########.fr       */
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
		setHeader("Server","WebServ/1.0");
		setHeader("Content-Type","text/html");
	}
	Response::~Response(){};

	std::string Response::getHeaders(Request &request, Config &config, std::string statusCode)
	{

		

		setDateHeader();	
		setContentLength(this->bodyPath);
		return (headerBuilder());
	}

	std::pair<std::string, bool> Response::getbody()
	{
		return std::pair<std::string,bool>(this->bodyPath,this->response_is_tmp);
	}


	std::string Response::headerBuilder()
	{
		std::stringstream headers;

		//build status line
		headers << "HTTP/1.1 "+ this->statusCode +" "+getMessage(this->statusCode)+"\r\n";
		//build headers
		std::map<std::string,std::string>::iterator it;
		for (it = this->headers_list.begin(); it != this->headers_list.end(); ++it)
    			headers << it->first << ": " << it->second << "\r\n";
		headers << "\r\n";
		
		return headers.str();
	}

	void Response::setDateHeader()
	{
		time_t curr_time;
		struct tm * tm;
		std::string days[] =  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
		std::string months [] = {"Jan", "Feb", "MAar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Ded"};
		// Mon, 06 Jun 2022 03:48:42 GMT
		time(&curr_time);
		tm = gmtime(&curr_time);
		std::stringstream date;
		date << days[tm->tm_wday].c_str() <<", " << std::setw(2) << std::setfill('0') << tm->tm_mday ;
		date << " " << months[tm->tm_mon].c_str() << " " << tm->tm_year+1900 <<" " ;
		date << std::setw(2) << std::setfill('0') <<(tm->tm_hour+0)%24 <<":"<< std::setw(2) << std::setfill('0') <<tm->tm_min <<":"<< std::setw(2) << std::setfill('0') << tm->tm_sec << " GMT";
		this->headers_list.insert(std::pair<std::string, std::string>("Date", date.str()));
	}

	void Response::setContentLength(std::string filePath)
	{
		if(!filePath.empty() && filePath.length() > 0)
			setHeader("Content-Length",std::to_string(getFileSize(filePath)));
	}

	void Response::setHeader(std::string key, std::string value)
	{
		this->headers_list.insert(std::make_pair(key, value));
	}

	long long Response::getFileSize(std::string filePath)
	{
		struct stat st;

		if(!filePath.empty() && filePath.length() > 0)
		{
			stat(filePath.c_str(), &st);
			return st.st_size;
		}
		return 0;
	}

	std::string Response::getMessage(std::string statusCode)
	{
		if(statusCodeMessages.find(statusCode) != statusCodeMessages.end())
			return statusCodeMessages.find(statusCode)->second;
		//else throw error
		return "";
	}
}