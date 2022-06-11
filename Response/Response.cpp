/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 03:04:46 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/11 02:18:15 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../Response/Response.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include <ctime> 
#include <vector>
#include <string>
#include <cstring>
#include <sstream>


namespace ws {
	Response::Response():buildResponseTry(0),response_is_tmp(false),response_ready(false)
	{
		setHeader("Server","WebServ/1.0");
		setHeader("Content-Type","text/html");
	}

	Response::~Response()
	{}

	//get headers and status line
	std::string Response::getHeaders(ws::Req req , ws::Location loc, int statusCode)
	{
		if(statusCode != -1)
		{
			buildResponse(std::to_string(statusCode), req , loc);
		}
		if(methodeAllowed(req, loc))
		{
			// check resource
			std::cout << "checking resource" << std::endl;
			// this->bodyPath = "files/root/test.html";//test
			// this->statusCode = "200";//test
			checkRedirections(req , loc); // TODO
		}
		else
		{
			buildResponse("405", req , loc);
		}

		setDateHeader();	
		setContentLength(this->bodyPath);
		return (headerBuilder());
	}


	void	Response::buildResponse(std::string statusCode,ws::Req req , ws::Location loc)
	{
		//check if errorpage exist
		bool error_pages = false;
		this->statusCode = statusCode;
		std::string errorPath = std::string();
		//search for error page path
		if(isErrorPage(loc,statusCode))//TODO what if the path not a file
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
					buildResponse("403", req , loc);
					return ;
				}
			}
		}
		//reset retry
		this->buildResponseTry = 0;

		if(!error_pages)
		{
			// call template
			std::cout << "===== call template "  << this->statusCode << " " << this->bodyPath<< std::endl;

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
		//build headers
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
			std::cout << "lvl of checking resource" << std::endl;
			checkResource(req, loc);
		}
		else
		{	
			//TODO need to be tested after the merge it need new location to work
			// getRedirectionStatus();//no need anymore
			// getRedirectionPath();
			// setHeader("Location",redirectionPath);
			// buildResponse("301", req , loc);
		}
	}


	void Response::checkResource(ws::Req req , ws::Location loc)
	{
		if(getMethode(req) == "POST" && getLocationPath(loc) == "/upload")
		{
			std::string filename = getUploadFileName(req);
			std::string filenamePath = builPath(loc, filename);
			std::cout << "info :" << getMethode(req) << " " << getLocationPath(loc) << " " <<  getResourcePath(req, loc) << " " << getUploadPath(loc)  << " "<< getUploadFileName(req)<<  std::endl;
			//TODO 
			// i will need a filename from request wich sholud be extracted from resource path
			// i will need a upload directory path from config 
			//
			//check if file name given exist  buildResponse("409", req , loc);// conflict // done
			//	if  not check if has directories on the path if those directories exist check there permission
			//	.		split resourcePath and get file name
			// 	.		upload request body to the right path // mv cmd
			// 	.		system("mv ./files/tmp/bodytesttmp.html ./files/upload/body.html");//working
			// 	.		check after mv if file correctly moved
			// 	.		.	building heders :
			// 	.		.	no need for body path for response make it empty
			// 	.		.	headers need location path of the uploaded resource
			// 	.		.	buildResponse("201", req , loc);
			//	.		if not buildResponse("500", req , loc);//
			//	else
			//		buildResponse("403", req , loc); forbidden
			if(ws::fileHandler::checkIfExist(filenamePath))
			{
				std::cout << "file exist"<< std::endl;
				buildResponse("409", req , loc);// conflict	
			}
			else
			{
				std::cout << "lets upload"<< std::endl;
				//split if theres is directories
				std::vector<std::string> dirList = pathSpliter(loc , filename);
				int el = directoriesHandler(loc , dirList[0], dirList, 0,filenamePath);
				if(el == 1)//directories path exist with no file
				{
					std::string cmd = "mv "+ getRequestBodyPath(req) +" " + filenamePath;
					int err = system(cmd.c_str());//working
					if(err)
						buildResponse("500", req , loc);
					else
					{
						setHeader("Location",filenamePath);
						this->bodyPath.clear();
						this->response_is_tmp = false;
						this->statusCode = "201";
					}
				}
				else
					buildResponse(std::to_string(el), req , loc);// conflict or server error ...	
			}
		}
		else if(isResourceDir(req, loc,getResourcePath(req,loc)))//TODO 01
		{
			// defineResourceType(req, loc);
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
		else
		{
			buildResponse("404", req , loc);
		}
	}

	// void Response::craftDeleteRequests(ws::Req req , ws::Location loc)
	// {
	// 	(void)req;
	// 	(void)loc;
	// }

	// void Response::defineResourceType(ws::Req req , ws::Location loc)
	// {
	// 	// if(isResourceDir(req, loc))
	// 	// {
	// 	// 	if(isUriValid(req, loc))
	// 	// 	{
	// 	// 		checkIndex(req, loc);
	// 	// 	}
	// 	// 	else if(isUriValid(req, loc) && getMethode(req) == "DELETE")
	// 	// 	{
	// 	// 		cgiChecker(req,loc);
	// 	// 	}
	// 	// 	else if(getMethode(req) == "DELETE")
	// 	// 	{
	// 	// 		// buildResponse("409",req,loc);
	// 	// 	}
	// 	// 	else
	// 	// 	{
	// 	// 		//fix uri first
	// 	// 		// buildResponse("301",req,loc);
	// 	// 	}
	// 	// }
	// 	// else if(isResourceFile(req, loc))
	// 	// {
	// 	// 	cgiChecker(req,loc);
	// 	// }
	// }

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

	std::vector<std::string> Response::pathSpliter(ws::Location loc , std::string filePath)
	{
		(void)loc;
		std::vector<std::string> dirList;
		char* token = strtok(const_cast<char*>(filePath.c_str()),"/");
		
		while (token != NULL)
		{	
			dirList.push_back(std::string(token));
			token = strtok (NULL, "/");			
		}
		return dirList;
	}

	int Response::directoriesHandler(ws::Location loc , std::string filename, std::vector<std::string> dirList, int i, std::string originPath)
	{
		int ret = 0;
		std::string path = builPath(loc, filename);
		if(i < (int)dirList.size() - 2)
		{	
			i++;
			std::string newf = filename+"/"+dirList[i];
			ret = directoriesHandler(loc, newf,dirList,i,originPath);
		}
		///TODO 0
		std::cout << path << std::endl;//DEBUG
		if(ret != 0)
			return ret;
		struct stat info;

		if( stat( path.c_str(), &info ) != 0 && (info.st_mode & S_IWUSR))
		{	
			std::cout << "doesnt exist "<< std::endl;
			size_t pos = originPath.find_last_of('/');
			std::string pathBuild = "mkdir -p "+ originPath.substr(0,pos);
			struct stat info2;
			stat( pathBuild.c_str(), &info2 );
			int err = system(pathBuild.c_str());
			if(err)
			{	
				std::cout << "have no w 000000"<< std::endl;
				return 403;
			}
			return 1;
		}
		else // if exist
		{
			if( info.st_mode & S_IFDIR )
			{	
				std::cout << "is a directory"<< std::endl;
				if(info.st_mode & S_IWUSR)
				{	
					std::cout << "have w "<< std::endl;
					return 1; // exist
				}
				else if (!(info.st_mode & S_IWUSR))
				{	
					std::cout << "have no right "<< std::endl;
					return 403;
				}
			}
			else
			{	
				std::cout << "is no directory"<< std::endl;
				// buildResponse("409", req , loc);// conflict	
				return 409;
			}
		}
		return ret;
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
		// std::cout << "size" << std::to_string(loc.redirection.size()) << std::endl;// DEBUG
		// return (loc.redirection.size() > 0);
		return (!loc.redirection.empty());
	}

	// bool Response::isResourceExist(ws::Req req , ws::Location loc)
	// {
	// 	(void)req;
	// 	(void)loc;
	// 	return false;
	// }

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

//TODO VVVVV
	bool Response::isResourceDir(ws::Req req , ws::Location loc,std::string resourcePath)
	{
		(void)req;
		(void)loc;
		struct stat info;

		if( stat( resourcePath.c_str(), &info ) == 0)
		{	
			if( info.st_mode & S_IFDIR )
			{	
				std::cout << "is a directory"<< std::endl;
				if(info.st_mode & S_IWUSR)
				{	
					std::cout << "have w "<< std::endl;
					// return 1; // exist
				}
				else if (!(info.st_mode & S_IWUSR))
				{	
					std::cout << "have no right "<< std::endl;
					// return 403;
				}
			}
			else
			{	
				std::cout << "is no directory"<< std::endl;
				// buildResponse("409", req , loc);// conflict	
				// return 409;
			}
		}
		else
		{
			//doesnt exist
		}
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
		// (void)req;
		(void)loc;
		
		return req.requestInfo.find("resource")->second;
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
	
	// std::string Response::getRedirectionStatus(ws::Location loc)
	// {
	// 	(void)loc;
	// 	return "";
	// }

	std::string Response::getRedirectionPath(ws::Location loc)
	{
		(void)loc;
		return "";
	}

	std::string Response::getLocationPath(ws::Location loc)
	{
		return loc.locationPath;
	}

	std::string Response::getUploadPath(ws::Location loc)
	{
		return loc.uploadPath;
	}

	std::string Response::getUploadFileName(ws::Req req)
	{
		return req.fileName;
	}

	void Response::setContentLength(std::string filePath)
	{
		if(!filePath.empty() && filePath.length() > 0)
			setHeader("Content-Length",std::to_string(getFileSize(filePath)));
	}

	void Response::setHeader(std::string key, std::string value)
	{
		this->headers_list.insert(std::pair<std::string, std::string>(key,value));
	}

	std::string Response::getRequestBodyPath(ws::Req req)
	{
		return req.bodyPath;
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