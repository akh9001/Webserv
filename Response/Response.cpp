/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 03:04:46 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/07 02:28:18 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../Response/Response.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include <ctime> 
#include <vector>
#include <string>
#include <sstream>

namespace ws {
	Response::Response()
	{
		//TODO
		this->response_is_tmp = false;
		this->response_ready = false;
		this->buildResponseTry = 0;

		this->headers_list.insert(std::pair<std::string, std::string>("Server","WebServ/1.0"));
		this->headers_list.insert(std::pair<std::string, std::string>("Content-Type","text/html"));
	}

	Response::~Response()
	{
	}

	//get headers and status line
	std::string Response::getHeaders(ws::Req req , ws::Location loc, int statusCode)
	{
		if(statusCode != -1)
		{
			buildError(std::to_string(statusCode), req , loc);
		}
		if(methodeAllowed(req, loc))
		{
			//check resource
			std::cout << "checking resource" << std::endl;
			this->bodyPath = "files/root/test.html";
			this->statusCode = "200";
			// checkRedirections(req , loc); // TODO
		}
		else
		{
			buildError("404", req , loc);
		}

		setDateHeader();	
		setContentLength(this->bodyPath);
		return (headerBuilder());
	}


	void	Response::buildError(std::string statusCode,ws::Req req , ws::Location loc)
	{
		//check if errorpage exist
		bool error_pages = false;
		this->statusCode = statusCode;
		std::string errorPath = std::string();
		//search for error page path
		if(isErrorPage(loc,statusCode))
		{
			//TODO get this into the isErrorPages function then flag error_pages (error_pages = isErrorPages(loc))
			//build error page path + root
			errorPath = builPath(loc, getErrorPage(loc, statusCode));
			//if error page exist
			if(ws::fileHandler::checkIfExist(errorPath))
			{	
				// check permission valid
				if(isFilePermission(errorPath, "r"))
				{
					std::cout << "page exist  with permession" << errorPath << std::endl;
					this->bodyPath = errorPath;
					error_pages = true;
				}
				else if(this->buildResponseTry == 0)
				{
					this->buildResponseTry++;
					error_pages = false;
					std::cout << "page exist with noooooo permession" << errorPath << std::endl;
					this->statusCode = "403";
					buildError("403", req , loc);
					return ;
				}
			}
		}
		//reset retry
		this->buildResponseTry = 0;

		if(!error_pages)
		{
			// call template
			std::cout << "===== call template"  << std::endl;
			std::string resPath;
			try
			{
				resPath = ws::fileHandler::createTmp("./files/tmp");
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			bodyDefaultTemplate(statusCode, resPath);
			this->bodyPath = resPath;
			this->response_is_tmp = true;
		}
	}

	std::string Response::headerBuilder()
	{
		std::stringstream headers;

		//build status line
		headers << "HTTP/1.1 "+ this->statusCode +" "+getMessage(this->statusCode)+"\r\n";
		std::map<std::string,std::string>::iterator it;
		for (it = this->headers_list.begin(); it != this->headers_list.end(); ++it)
    			headers << it->first << ": " << it->second << "\r\n";
		headers << "\r\n";
		
		return headers.str();
	}


	bool Response::methodeAllowed(ws::Req req , ws::Location loc)
	{
		std::string reqMethod = getMethode(req);
		return find(loc.allowedMethod.begin(), loc.allowedMethod.end(), reqMethod) != loc.allowedMethod.end();
	}

	void Response::checkRedirections(ws::Req req , ws::Location loc)
	{
		if(!isRedirection(loc))
		{
			craftResponse(req, loc);
		}
		else
		{
			// buildResponse("301",req,loc);//
		}
	}


	void Response::craftResponse(ws::Req req , ws::Location loc)
	{
		if(getMethode(req) == "POST" )//and location is /upload
		{
			//upload request body to the right path
			// buildResponse("201",req,loc);//
		}
		else if(isResourceExist(req, loc))
		{
			defineResourceType(req, loc);
		}
		else
		{
			// buildResponse("404",req,loc);//
		}
	}

	void Response::craftDeleteRequests(ws::Req req , ws::Location loc)
	{
		(void)req;
		(void)loc;
	}

	void Response::defineResourceType(ws::Req req , ws::Location loc)
	{
		if(isResourceDir(req, loc))
		{
			if(isUriValid(req, loc))
			{
				checkIndex(req, loc);
			}
			else if(isUriValid(req, loc) && getMethode(req) == "DELETE")
			{
				cgiChecker(req,loc);
			}
			else if(getMethode(req) == "DELETE")
			{
				// buildResponse("409",req,loc);
			}
			else
			{
				//fix uri first
				// buildResponse("301",req,loc);
			}
		}
		else if(isResourceFile(req, loc))
		{
			cgiChecker(req,loc);
		}
	}

	void Response::cgiChecker(ws::Req req , ws::Location loc)
	{
		if(isCGI(loc))
		{
			if(getMethode(req) == "DELETE" )
			{
				checkIndex(req, loc);
			}
			else
				cgiHandler(req ,loc);
		}
		else if(getMethode(req) == "POST" )
		{
			//return requested file
			// buildResponse("403",req,loc);
		}
		else if(getMethode(req) == "DELETE" )
		{
			//deletefile
			//if success
				// buildResponse("204",req,loc);
			//else
				//check permission allowed
					// buildResponse("500",req,loc);
				//else
					// buildResponse("403",req,loc);
		}
		else
		{	
			//return requested file
			// buildResponse("200",req,loc);
		}
	}

	void Response::cgiHandler(ws::Req req , ws::Location loc)
	{
		(void)req;
		(void)loc;
	}

	void Response::checkIndex(ws::Req req , ws::Location loc)
	{
		if(isIndex(loc) && getMethode(req) == "DELTE" )
		{
			cgiHandler(req ,loc);
		}
		else if(isIndex(loc))//and if file index exist in this location dir
		{
			cgiChecker(req,loc);
		}
		else if(getMethode(req) != "GET" )
		{
			// buildResponse("403",req,loc);
		}
		else
		{
			checkAutoIndex(req, loc);
		}
	}

	void Response::checkAutoIndex(ws::Req req , ws::Location loc)
	{
		(void)req;
		if(getAutoIndex(loc) == "off")
		{
			// buildResponse("403",req,loc);//
		}
		else if (getAutoIndex(loc) == "on")
		{
			//build auto index as resourse
			// buildResponse("200",req,loc);//
		}
	}

	std::string Response::builPath(ws::Location loc, std::string resourcePath)
	{
		std::string backSlash;
		if(resourcePath.at(0) != '/')
			backSlash = "/";
		std::string path = loc.root + backSlash + resourcePath;
		return path;
	}

	void Response::bodyDefaultTemplate(std::string statusCode, std::string resPath)
	{
		std::string message = getMessage(statusCode);
		std::string response(
				"<html>"
					"<head>"
						"<title>"+ statusCode + " " +message +"</title>"
					"</head>"
					"<body>"
						"<center>"
							"<h1>"+ statusCode + " " +message +"</h1>"
						"</center>"
						"<hr>"
						"<center>webserv/1.1</center>"
					"</body>"
				"</html>\r\n\r\n");
		fileHandler::write(resPath,response);
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

	bool Response::isRedirection(ws::Location loc)
	{
		(void)loc;
		return false;
	}

	bool Response::isResourceExist(ws::Req req , ws::Location loc)
	{
		(void)req;
		(void)loc;
		return false;
	}

	bool Response::isCGI(ws::Location loc)
	{
		(void)loc;
		return false;
	}

	bool Response::isUriValid(ws::Req req , ws::Location loc)
	{
		(void)req;
		(void)loc;
		return false;
	}

	bool Response::isIndex(ws::Location loc)
	{
		(void)loc;
		return false;
	}

	bool Response::isErrorPage(ws::Location loc, std::string statusCode)
	{
		return (loc.errorPages.find(statusCode) != loc.errorPages.end());
	}

	bool Response::isResourceDir(ws::Req req , ws::Location loc)
	{
		(void)req;
		(void)loc;
		return false;
	}

	bool Response::isResourceFile(ws::Req req , ws::Location loc)
	{
		(void)req;
		(void)loc;
		return false;
	}

	bool Response::isFilePermission(std::string path, std::string permission)
	{
		(void)permission;
		struct stat fileStat;

		if(stat(path.c_str(),&fileStat) < 0)    
			return false;

		// printf("File Permissions: \n");
		// printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
		// printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
		// printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
		// printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
		// printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
		// printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
		// printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
		// printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
		// printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
		// printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
		// printf("\n");
		if(permission == "r")
			return (fileStat.st_mode & S_IRUSR);
		if(permission == "w")
			return (fileStat.st_mode & S_IWUSR);
		if(permission == "x")
			return (fileStat.st_mode & S_IXUSR);
		
		return false;
	}


	std::string Response::getResourcePath(ws::Req req, ws::Location loc)
	{
		(void)req;
		(void)loc;
		return std::string();
	}

	std::string  Response::getMethode(ws::Req req)
	{
		return req.requestInfo.find("method")->second;
	}

	std::string Response::getAutoIndex(ws::Location loc)
	{
		(void)loc;
		return std::string();
	}

	std::string Response::getErrorPage(ws::Location loc, std::string statusCode)
	{
		return loc.errorPages.find(statusCode)->second;
	}

	std::string Response::getMessage(std::string statusCode)
	{
		if(statusCodeMessages.find(statusCode) != statusCodeMessages.end())
			return statusCodeMessages.find(statusCode)->second;
		//else throw error
		return "";
	}

	std::pair<std::string,bool> Response::getbody()
	{
		return std::pair<std::string,bool>(this->bodyPath,this->response_is_tmp);
	}

	long long Response::getFileSize(std::string filePath)
	{
		struct stat st;
		stat(filePath.c_str(), &st);
		return st.st_size;
	}
	
	void Response::setContentLength(std::string filePath)
	{
		this->headers_list.insert(std::pair<std::string, std::string>("Content-Length",std::to_string(getFileSize(filePath))));
	}

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
}