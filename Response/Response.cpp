/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:59 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/16 00:23:22 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <ctime> 
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <iterator>
#include <unistd.h>

namespace ws {

	Response::Response():buildResponseTry(0),response_is_tmp(false)
	{
		init_statusCodeMessages();
		setHeader("Server","WebServ/1.0");
		setHeader("Content-Type","text/html");
	}
	Response::~Response(){};

	std::string Response::getHeaders(Request &request,Location &location, std::string &statusCode)
	{
		//set current location and statuscode
		this->currentLocation = location;
		this->statusCode = statusCode;
		int status;
		std::istringstream(statusCode) >> status;

		// 
		//check if exist
		if(statusCode != "-1" && status >= 400)
		{	
			buildResponse(request);
		}
		else
		{
			checkResource(request);
		}

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

	void	Response::buildResponse(Request &request)
	{
		// check if errorpage exist
		bool error_pages = false;
		std::string originErrorPath = std::string();
		std::cout << this->statusCode << " " << getErrorPage() << " page |" << getErrorPage() <<"|"<< std::endl;
		//search for error page path
		
		if(isErrorPage())//
		{
			originErrorPath = getErrorPage();
			// originErrorPath = "/dir";
			// std::cout << "Working on this error page" << std::endl;
			if(originErrorPath.at(0) == '/')
			{
				std::string errorPath = builPath(originErrorPath);
				std::cout << "start with / "<< errorPath << std::endl;
				//if errorPath source exist in root
				if(ws::fileHandler::checkIfExist(errorPath))
				{
					// std::cout << errorPath << " exist"<< std::endl;
					// check permission valid
					if(isPermission(errorPath, "r"))
					{
						if(isFile(errorPath))// else if file
						{
							// std::cout << errorPath << " is a file "<< std::endl;
							error_pages = true;
							this->bodyPath = errorPath;
						}
						else
						{
							//directories not implimented
							error_pages = false;
							this->statusCode = "501";
						}
					}
					else
					{
						// std::cout << errorPath << " have no permission "<< std::endl;
						// std::cout << "403" << std::endl;
						error_pages = false;
						this->statusCode = "403";
					}
				}
				else
				{
					// std::cout << errorPath << " not exist"<< std::endl;
					// std::cout << "404" << std::endl;
					error_pages = false;
					this->statusCode = "404";
				}
			}
			else
			{
				// std::cout << "302" << std::endl;
				error_pages = false;
				this->statusCode = "302";
				// std::cout << "Location: " << originErrorPath << std::endl;
				setHeader("Location",originErrorPath);
			}
		}

		this->buildResponseTry = 0;

		if(!error_pages)
		{
			// call template
			std::string responsePath;
			try
			{
				responsePath = ws::fileHandler::createTmp("./response_tmp_files");
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			bodyDefaultTemplate(responsePath);
			this->bodyPath = responsePath;
			this->response_is_tmp = true;
		}

	}

	void Response::bodyDefaultTemplate(std::string &responsePath)
	{
		std::string message = getMessage(this->statusCode);
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
		fileHandler::write(responsePath,response);
	}

	std::string Response::builPath(std::string &resourcePath)
	{
		std::string backSlash;
		std::string root;
		char tmp[2048];
		getcwd(tmp, 2048);

		if(resourcePath.at(0) != '/')
			backSlash = "/";
		root = this->currentLocation.getRoot();
		root  = ltrim(root);
		std::string path =   std::string(tmp) + root + backSlash + resourcePath;
		return path;
	}

	

	void Response::checkResource(Request &request)
	{
		
		if(request.getRightLocation() == 0)//location doesnt exist
		{
			// std::cout << "location wrong"<< std::endl;
			// get root location
			// this->statusCode = "404";
			//search for location
			Server s = request.getServer();
			std::vector<Location> locs = s.getLocation();
			for (std::vector<Location>::iterator it = locs.begin(); it != locs.end() ;++it)
			{
				Location  l1= *it;
				if(l1.getLocation_match() == "/")
				{	
					std::cout << l1.getLocation_match() << std::endl;
					this->currentLocation = *it;
					break;
				}
			}
		}
		
		
		//check methode allowed
		if(isMethodeAllowed(request))
		{
			// std::cout << "right location and right methodes"<< std::endl;
			if(!isRedirection())
			{
				// std::cout << "status now " << this->statusCode << std::endl;
				// std::cout << "no redirection"<< std::endl;
				//check status code
				// if(this->statusCode != "-1")
				// {
				// 	buildResponse(request);
				// }
				// else
				// {
					std::cout << "define method "<< request.getUri() << std::endl;
					defineMethode(request);					
				// }
			}
			else //TODO redirection function
			{
				//before redirecting
				//if code between 3xx 
				//	check if error page 
				//	if not redirect to that path
				//else
				//	put code as status and path as body
				// std::cout << "redirection"<< std::endl;
				std::string redirectionPath = this->currentLocation.getRedirectUri().find(301)->second;
				this->statusCode = "301";
				setHeader("Location",redirectionPath);
				buildResponse(request);
			}
		}
		else
		{
			this->statusCode = "405";
			buildResponse(request);
		}
	}


	void Response::defineMethode(Request &request)
	{
		if(getMethod(request) == "GET")
		{
			std::cout <<"GET crafting "<< std::endl;
			craftGetRequests(request);
		}
		else if(getMethod(request) == "POST")
		{
			//TODO
		}
		else if(getMethod(request) == "DELETE")
		{
			//TODO
		}
	}

	void	Response::craftGetRequests(Request &request)
	{
		//check resource if exist
		std::string requestResource = request.getUri();
		std::string absoluteResourcePath = builPath(requestResource);
		//if errorPath source exist in root
		if(ws::fileHandler::checkIfExist(absoluteResourcePath))
		{
			std::cout << "GET "<< absoluteResourcePath <<" exist "<< ws::fileHandler::checkIfExist(absoluteResourcePath) << std::endl;
			// check permission valid
			if(isPermission(absoluteResourcePath, "r"))
			{
				if(isDir(absoluteResourcePath))
				{
					
					std::cout << "test dir" << std::endl;
					int endPos = requestResource.length();
					--endPos;
					if(requestResource.at(endPos) != '/')
					{
						std::cout << "redirect" << std::endl;
						this->statusCode = "301";
						setHeader("Location",requestResource+"/");
						buildResponse(request);
					}
					else
					{
						///search in locations
						Server s = request.getServer();
						std::vector<Location> locs = s.getLocation();
						std::vector<Location>::iterator it;
						for (it = locs.begin(); it != locs.end() ;++it)
						{
							Location  l1= *it;
							if(l1.getLocation_match() == rtrim(requestResource))
							{	
								// std::cout << l1.getLocation_match() << std::endl;
								this->currentLocation = *it;
								break;
							}
						}
						// std::cout << this->currentLocation.getLocation_match() << std::endl;
						if(it == locs.end()) //no location
						{
							this->statusCode = "403";
							buildResponse(request);
						}
						else
						{
							//TODO
							//check on indexs
							if(isIndexes()) 
							{
								bool isIndex = false;
								std::vector<std::string> indexList = getIndexes();
								for (size_t i = 0; i < indexList.size(); i++)
								{
									std::string indexPath = absoluteResourcePath+indexList[i];
									std::cout << "index is " << indexPath << std::endl;
									if(ws::fileHandler::checkIfExist(indexPath))
									{
										if(isPermission(indexPath, "r"))
										{
											if(isDir(indexPath))
											{
												std::cout << "index is dir" << std::endl;
												this->statusCode = "501";
												buildResponse(request);
												isIndex = true;
												break;
											}
											else if(isFile(indexPath))
											{
												std::cout << "index is file" << std::endl;
												//TODO
												// check if cgi
												// else
													this->statusCode = "200";
													this->bodyPath = indexPath;
												isIndex = true;
												break;
											}
										}
									}
								}
								if(!isIndex)
								{
									if(isAutoIndexOn())
									{
										//TODO
										std::cout << "index is autoindex" << std::endl;
										this->statusCode = "200";
										//build autoindex and push it to bodypath and tmp true
										this->bodyPath = "/Users/laafilal/Desktop/webserv1/autoindex.html";
									}
									else
									{
										std::cout << "index is 403 0" << std::endl;
										this->statusCode = "403";
										buildResponse(request);
									}
								}
							}
							else
							{
								std::string defaultIndexPath = absoluteResourcePath+"index.html";
								
								std::cout << "index is exist " << ws::fileHandler::checkIfExist(defaultIndexPath)<< std::endl;
								if(ws::fileHandler::checkIfExist(defaultIndexPath))
								{
									//TODO

									//check permission
									// else
									// this->statusCode = "403";
									// buildResponse(request);
									std::cout << "index is index.html " << defaultIndexPath<< std::endl;
									this->statusCode = "200";
									this->bodyPath = defaultIndexPath;
								}
								else
								{
									if(isAutoIndexOn())
									{
										//TODO
										//build autoindex and push it to bodypath and tmp true
										std::cout << "index is autoindex" << std::endl;
										this->statusCode = "200";
										this->bodyPath = "/Users/laafilal/Desktop/webserv1/autoindex.html";
									}
									else
									{
										std::cout << "index is 403 1" << std::endl;
										this->statusCode = "403";
										buildResponse(request);
									}
								}
							}
						}
					}
				}
				else if(isFile(absoluteResourcePath))// else if file
				{
					//TODO
					//check cgi
					// ...
					//else
					std::cout << "test file" << std::endl;
					this->statusCode = "200";
					this->bodyPath = absoluteResourcePath;
				}
			}
			else
			{
				this->statusCode = "403";
				buildResponse(request);
			}
		}
		else
		{
			
			this->statusCode = "404";
			buildResponse(request);
		}
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

	long long Response::getFileSize(std::string &filePath)
	{
		struct stat st;

		if(!filePath.empty() && filePath.length() > 0)
		{
			stat(filePath.c_str(), &st);
			return st.st_size;
		}
		return 0;
	}

	std::string Response::getMessage(std::string &statusCode)
	{
		if(statusCodeMessages.find(this->statusCode) != statusCodeMessages.end())
			return statusCodeMessages.find(this->statusCode)->second;
		//else throw error
		return "";
	}

	std::string Response::getErrorPage()
	{
		int status;
		std::istringstream(this->statusCode) >> status;
		std::map<int, std::string> errorPagesList = this->currentLocation.getErrorPages();
		return errorPagesList.find(status)->second;
	}

	std::string  Response::getMethod(Request &request)
	{
		return request.getMethod();
	}

	std::vector<std::string> Response::getIndexes()
	{
		return this->currentLocation.getIndex();
	}

	bool Response::isMethodeAllowed(Request &request)
	{

		std::vector<std::string> Methods = this->currentLocation.getAllowedMethods();
		return (find(Methods.begin(),Methods.end(), getMethod(request)) != Methods.end());
	}


	bool Response::isErrorPage()
	{
		int status;
		std::istringstream(this->statusCode) >> status;
		std::map<int, std::string> errorPagesList = this->currentLocation.getErrorPages();
		return ((errorPagesList.size() > 0) && (errorPagesList.find(status) != errorPagesList.end()));
	}

	bool Response::isPermission(std::string &path, std::string permission)
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

	bool Response::isDir(std::string &resourcePath)
	{
		struct stat info;

		stat( resourcePath.c_str(), &info );
		return (info.st_mode & S_IFDIR);
	}
	
	bool Response::isFile(std::string &resourcePath)
	{
		struct stat info;

		stat( resourcePath.c_str(), &info );
		return (info.st_mode & S_IFREG);
	}

	bool Response::isRedirection()
	{
		return (this->currentLocation.getRedirectUri().size() > 0);
	}

	bool Response::isIndexes()
	{
		return (this->currentLocation.getIndex().size() > 0);
	}

	bool Response::isAutoIndexOn()
	{
		return (this->currentLocation.getAutoIndex() == true);
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

	const std::string WHITESPACE = " \n\r\t\f\v./";
 
	std::string ltrim(const std::string &s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}
	
	std::string rtrim(const std::string &s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}
	
	std::string trim(const std::string &s) {
		return rtrim(ltrim(s));
	}
}