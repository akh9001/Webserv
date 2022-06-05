/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 03:04:46 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/05 01:56:25 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../Response/Response.hpp"
#include "Request.hpp"
#include "Location.hpp"

ws::Response::Response()
{
	this->response_is_tmp = false;
	this->response_ready = false;
	this->resourcePath = std::string();
	this->buildResponseTry = 0;
}

ws::Response::~Response()
{
}

std::string ws::Response::getResponse(ws::Req req , ws::Location loc, int statusCode)
{
	if(statusCode != -1) //errors befor the parsing request complately
	{
		buildResponse(std::to_string(statusCode),req,loc);
		return this->resPath;
	}
	if(methodeAllowed(req, loc))
	{
		checkRedirections(req , loc);
		return this->resPath;
	}
	buildResponse("404",req,loc);//
	return this->resPath;
};

void ws::Response::buildResponse(std::string statusCode, ws::Req req , ws::Location loc)
{
	bool error_pages = false;
	(void)statusCode;
	(void)req;
	(void)loc;
	//search for error page path
	//build error page path + root
	//if error page exist
	//	check permission valid
	//		flag eror page true
	//	not valid permission
	//	  	403 forbidden ^^^ recall same function
	
	//search for error page path
	if(statusCode != "-1" && isErrorPages(loc))
	{
		//TODO get this into the isErrorPages function then flag error_pages (error_pages = isErrorPages(loc))
		//build error page path + root
		std::string errorPath = builPath(loc, getErrorPage(loc, statusCode));
		//if error page exist
		if(ws::fileHandler::checkIfExist(errorPath))
		{	
			// check permission valid
			if(isFilePermission(errorPath, "r"))
			{
				std::cout << "pages got permession" << errorPath << std::endl;
				error_pages = true;
			}
			else if(this->buildResponseTry == 0)
			{
				this->buildResponseTry++;
				error_pages = false;
				std::cout << "pages got noooooo permession" << errorPath << std::endl;///403 forbidden ^^^ recall same function //problem infinit recursion
				buildResponse("403", req , loc);
				return ;
			}
		}
	}


	//build tmp response path
	// try
	// {
	// 	std::string resPath = ws::fileHandler::createTmp("./files/tmp");
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	
	//build status response
	//
	//build headers response ??????
	//
	
	//body building
	if(error_pages)
	{
		//body read from error file
	}
	else
	{
		if(statusCode == "-1" && !this->resourcePath.empty())
		{
			// body read from resource
		}
		else
		{
			// call template
		}
	}

	//set respath
}
//body building
//
//	if error page exist
//		check permission valid
//			body read from error file
//		not valid permission
//			clean all and delete files tmp
//	  		403 forbidden ^^^ recall same function
//	else 
//		if resourcePath not empty 
//			body read from resource
//		else body of simple template
//			call template
//body building
//
//	if error page true
//		body read from error file
//	else 
//		if statusCode == -1 && resourcePath not empty 
//			body read from resource
//		else body of simple template
//			call template
//
bool ws::Response::methodeAllowed(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
	return false;
}

void ws::Response::checkRedirections(ws::Req req , ws::Location loc)
{
	if(!isRedirection(loc))
	{
		craftResponse(req, loc);
	}
	else
	{
		buildResponse("301",req,loc);//
	}
}


void ws::Response::craftResponse(ws::Req req , ws::Location loc)
{
	if(getMethode(req) == "POST" )//and location is /upload
	{
		//upload request body to the right path
		buildResponse("201",req,loc);//
	}
	else if(isResourceExist(req, loc))
	{
		defineResourceType(req, loc);
	}
	else
	{
		buildResponse("404",req,loc);//
	}
}

void ws::Response::craftDeleteRequests(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
}

void ws::Response::defineResourceType(ws::Req req , ws::Location loc)
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
			buildResponse("409",req,loc);
		}
		else
		{
			//fix uri first
			buildResponse("301",req,loc);
		}
	}
	else if(isResourceFile(req, loc))
	{
		cgiChecker(req,loc);
	}
}

void ws::Response::cgiChecker(ws::Req req , ws::Location loc)
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
		buildResponse("403",req,loc);
	}
	else if(getMethode(req) == "DELETE" )
	{
		//deletefile
		//if success
			buildResponse("204",req,loc);
		//else
			//check permission allowed
				buildResponse("500",req,loc);
			//else
				buildResponse("403",req,loc);
	}
	else
	{	
		//return requested file
		buildResponse("200",req,loc);
	}
}

void ws::Response::cgiHandler(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
}

void ws::Response::checkIndex(ws::Req req , ws::Location loc)
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
		buildResponse("403",req,loc);
	}
	else
	{
		checkAutoIndex(req, loc);
	}
}

void ws::Response::checkAutoIndex(ws::Req req , ws::Location loc)
{
	if(getAutoIndex(loc) == "off")
	{
		buildResponse("403",req,loc);//
	}
	else if (getAutoIndex(loc) == "on")
	{
		//build auto index as resourse
		buildResponse("200",req,loc);//
	}
}

std::string ws::Response::builPath(ws::Location loc, std::string resourcePath)
{
	std::string backSlash;
	if(resourcePath.at(0) != '/')
		backSlash = "/";
	std::string path = loc.root + backSlash + resourcePath;
	return path;
}

bool ws::Response::isRedirection(ws::Location loc)
{
	(void)loc;
	return false;
}

bool ws::Response::isResourceExist(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
	return false;
}

bool ws::Response::isCGI(ws::Location loc)
{
	(void)loc;
	return false;
}

bool ws::Response::isUriValid(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
	return false;
}

bool ws::Response::isIndex(ws::Location loc)
{
	(void)loc;
	return false;
}

bool ws::Response::isErrorPages(ws::Location loc)
{
	return (loc.errorPages.size() > 0);
}

bool ws::Response::isResourceDir(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
	return false;
}

bool ws::Response::isResourceFile(ws::Req req , ws::Location loc)
{
	(void)req;
	(void)loc;
	return false;
}

bool ws::Response::isFilePermission(std::string path, std::string permission)
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


std::string ws::Response::getResourcePath(ws::Req req, ws::Location loc)
{
	(void)req;
	(void)loc;
	return std::string();
}

std::string  ws::Response::getMethode(ws::Req req)
{
	(void)req;
	return std::string();
}

std::string ws::Response::getAutoIndex(ws::Location loc)
{
	(void)loc;
	return std::string();
}

std::string ws::Response::getErrorPage(ws::Location loc, std::string statusCode)
{
	return loc.errorPages.find(statusCode)->second;
}

void ws::init_statusCodeMessages()
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
