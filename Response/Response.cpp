/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:59 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/30 19:20:27 by laafilal         ###   ########.fr       */
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
#include <dirent.h>
#include "../Includes/networking.hpp"

namespace ws {

	Response::Response():response_is_tmp(false)
	{
		setHeader("Server","WebServ/1.0");
		setHeader("Content-Type","application/octet-stream");
	}

	Response::~Response(){};

	std::string Response::getHeaders(Request &request,Location &location, std::string &statusCode)
	{
		this->currentLocation = location;
		this->statusCode = statusCode;
		int status;
		std::istringstream(statusCode) >> status;
		if(statusCode != "-1" && status >= 400)
		{	
			buildResponse(request);
		}
		else
		{
			try
			{
				checkResourceLocation(request);
				checkRedirection(request);
				checkAllowedMethods(request);
				defineMethode(request);
			}
			catch(const char *msg)
			{
				std::cout << "Log : ["<< msg <<"]" <<std::endl;
			}
		}

		setDateHeader();	
		setContentLength(this->bodyPath);
		return (headerBuilder());
	}


	std::string Response::headerBuilder()
	{
		std::stringstream headers;
		std::map<std::string,std::string>::iterator it;

		headers << "HTTP/1.1 "+ this->statusCode +" "+getMessage(this->statusCode)+"\r\n";
		for (it = this->headers_list.begin(); it != this->headers_list.end(); ++it)
    			headers << it->first << ": " << it->second << "\r\n";
		headers << "\r\n";
		
		return headers.str();
	}

	void	Response::buildResponse(Request &request)
	{
		bool error_pages = false;
		std::string originErrorPath = std::string();
		std::string msg;
		
		if(isErrorPage())
		{
			originErrorPath = getErrorPage();
			if(originErrorPath.at(0) == '/')
			{
				std::string errorPath = buildPath(originErrorPath);

				if(ws::fileHandler::checkIfExist(errorPath))
				{
					if(isFile(errorPath))
					{
						if(!isPermission(errorPath, "r"))
						{
							error_pages = false;
							this->statusCode = "403";
						}
						else
						{
							error_pages = true;
							this->bodyPath = errorPath; 
							setContentType(errorPath);
							throw "Error page was delivered successfully";
						}
					}
					else
					{
						error_pages = false;
						this->statusCode = "501";
					}
				}
				else
				{
					error_pages = false;
					this->statusCode = "404";
				}
			}
			else
			{
				this->statusCode = "302";
				originErrorPath = ltrim(originErrorPath);
				originErrorPath = buildLocationPath(originErrorPath, request);
				setHeader("Location",originErrorPath);
				throw "Redirection";
			}
		}

		if(!error_pages)
		{
			std::string responsePath;
			try
			{
				responsePath = ws::fileHandler::createTmp("./tmp");
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			bodyDefaultTemplate(responsePath);
			this->bodyPath = responsePath;
			setHeader("Content-Type","text/html");
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

	std::string Response::buildPath(std::string &resourcePath)
	{
		std::string root;

		root = this->currentLocation.getRoot() + "/";
		std::string path = resourcePath.replace(0,this->currentLocation.getLocation_match().length(),root);

		return path;
	}

	std::string Response::buildAbsolutePath(Request &request)
	{
		std::string requestResource = request.getUri();
		return buildPath(requestResource);
	}
	

	void Response::checkResourceLocation(Request &request)
	{
		if(request.getRightLocation() == 0)
			searchForLocation(request);
	}

	void Response::checkRedirection(Request &request)
	{
		if(isRedirection())
		{
			std::string redirectionPath = getRedirection().second;
			int status = getRedirection().first;
			this->statusCode = std::to_string(status);
			if(status >= 300 && status < 400)
			{	
				redirectionPath = ltrim(redirectionPath);
				redirectionPath = buildLocationPath(redirectionPath, request);
				setHeader("Location",redirectionPath);
				buildResponse(request);
				throw "Redirection";
			}
		}
	}
	
	void Response::checkAllowedMethods(Request &request)
	{
		if(isMethodeAllowed(request))
		{
			checkRoot(request);
		}
		else
		{
			this->statusCode = "405";
			buildResponse(request);
			throw "Method not allowed";
		}
	}

	void Response::defineMethode(Request &request)
	{

		if(getMethod(request) == "GET")
		{
			craftGetResponse(request);
		}
		else if(getMethod(request) == "POST")
		{
			craftPostResponse(request);
		}
		else if(getMethod(request) == "DELETE")
		{
			craftDeleteResponse(request);
		}
	}
	void Response::isResourceEndSlash(Request &request)
	{
		int endPos = request.getUri().length();
		--endPos;
		if(request.getUri().at(endPos) != '/')
		{
			this->statusCode = "301";
			std::string redirectionPath = ltrim(request.getUri());
			redirectionPath = buildLocationPath(redirectionPath, request);
			setHeader("Location",redirectionPath+"/");
			buildResponse(request);
			throw "Redirection";
		}
	}

	void Response::checkIndexes(Request &request)
	{
		std::vector<std::string> indexList = getIndexes();
		for (size_t i = 0; i < indexList.size(); i++)
		{
			std::string indexPath = buildAbsolutePath(request)+indexList[i];
			if(ws::fileHandler::checkIfExist(indexPath))
			{
				if(isDir(indexPath))
				{
					this->statusCode = "501";
					buildResponse(request);
					throw "Index should be a file";
				}
				else if(isFile(indexPath))
				{
					if(isPermission(indexPath, "r"))
					{
						if(isCgi())
						{
							request.cgi_ptr = new CGI();
							request.cgi_ptr->cgi(request, getCgiPath().c_str(), indexPath.c_str());
							throw "Calling cgi";
						}
						this->statusCode = "200";
						setContentType(indexPath);
						this->bodyPath = indexPath;
						setContentType(indexPath);
						throw "Index was delivered successfully";
					}
				}
			}
		}
		if(isAutoIndexOn())
		{
			autoIndexHandler(request);
		}
		else
		{
			this->statusCode = "403";
			buildResponse(request);
			throw "Index have an issue";
		}
	}

	void	Response::checkDefaultIndex(std::string &absoluteResourcePath, Request &request)
	{
		std::string defaultIndexPath = absoluteResourcePath+"index.html";
						
		if(ws::fileHandler::checkIfExist(defaultIndexPath))
		{	
			if(!isPermission(defaultIndexPath,"r"))
			{
				this->statusCode = "403";
				buildResponse(request);
				throw "Default index.html have no permissions";
			}

			this->statusCode = "200";
			this->bodyPath = defaultIndexPath;
			setHeader("Content-Type","text/html");
			throw "Default index was delivered successfully";
 		}
	}

	void	Response::craftGetResponse(Request &request)
	{
		std::string absoluteResourcePath = buildAbsolutePath(request);
		try
		{
			isResourceValid(absoluteResourcePath,request);
		}
		catch(const char* msg)
		{
			throw msg;
		}
		if(isDir(absoluteResourcePath))
		{
			if(!isPermission(absoluteResourcePath, "x"))
			{
				this->statusCode = "403";
				buildResponse(request);
				throw "Resource have no permissions";
			}
			isResourceEndSlash(request);
			searchForLocation(request);
			if(isIndexes()) 
			{
				try
				{
					checkIndexes(request);
				}
				catch(const char* msg)
				{
					throw msg;
				}
			}
			else 
			{
				checkDefaultIndex(absoluteResourcePath, request);
				if(isAutoIndexOn())
				{
					autoIndexHandler(request);
				}
				else
				{
					this->statusCode = "403";
					buildResponse(request);
					throw "Index have an issue";
				}
			}
		}
		else if(isFile(absoluteResourcePath))
		{
			if(!isPermission(absoluteResourcePath, "r"))
			{
				this->statusCode = "403";
				buildResponse(request);
				throw "Resource have no permissions";
			}
			if(isCgi())
			{
				request.cgi_ptr = new CGI();
				request.cgi_ptr->cgi(request, getCgiPath().c_str(), absoluteResourcePath.c_str());
				throw "Calling cgi";
			}
			this->statusCode = "200";
			setContentType(absoluteResourcePath);
			this->bodyPath = absoluteResourcePath;
			setContentType(absoluteResourcePath);
			throw "Resource was delivered successfully";
		}
	}

	void Response::checkRoot(Request &request)
	{
		if(this->currentLocation.getRoot().empty())
		{
			this->statusCode = "404";
			buildResponse(request);
			throw "There is no root";
		}
		else
		{
			std::string root = this->currentLocation.getRoot();
			if(!ws::fileHandler::checkIfExist(root))
			{
				this->statusCode = "404";
				buildResponse(request);
				throw "Root doesnt exist";
			}
			if(!isPermission(root, "x"))
			{
				this->statusCode = "403";
				buildResponse(request);
				throw "Root doesnt have permissions";
			}
		}
	}

	void Response::autoIndexHandler(Request &request)
	{
		std::string tmpDirectory;
		std::string tmpDirectoryPath;
		std::string tmpPath;
		std::multimap<std::string, std::pair<struct stat , long long> > dirList;

		DIR *dir;
		struct dirent *ent;
		std::string currLoc = request.getUri();
		std::string absPath = buildPath(currLoc);
		std::string slash;

		if ((dir = opendir(absPath.c_str())) != NULL) 
		{
			while ((ent = readdir(dir)) != NULL) {
				std::string filePath = absPath+"/"+std::string(ent->d_name);
				stat(filePath.c_str(), &this->fileStat);
				if(isDir(filePath))
					slash = "/";
				else
					slash = "";
				dirList.insert(std::make_pair(std::string(ent->d_name)+slash,std::make_pair(this->fileStat,getFileSize(filePath))));
			}
			closedir(dir);
		} else {
			throw "Could not open directory";
		}
		
		tmpDirectory = "./tmp";
		tmpPath = ws::fileHandler::createTmp(tmpDirectory);
		
		autoIndexTemplate(dirList,tmpPath);
		this->statusCode = "200";
		this->bodyPath = tmpPath;
		setHeader("Content-Type","text/html");
		this->response_is_tmp = true;
		throw "Autoindex was delivered successfully";
	}

	void	Response::autoIndexTemplate(std::multimap<std::string, std::pair<struct stat , long long> > &dirList, std::string filePath)
	{
		std::multimap<std::string, std::pair<struct stat , long long> >::iterator git;
		std::stringstream index;

		index << 	"<html><head>"
					"<title>Index of "+ this->currentLocation.getLocation_match()+"</title></head>"
					"<body>"
					"<h1>Index of "+ this->currentLocation.getLocation_match()+"</h1>"
					"<hr><pre>";
		
		for (git = dirList.begin(); git != dirList.end(); ++git)
		{
			index << "<a href='"+git->first+"'>"+git->first+"</a>";
			index <<  std::setw(40 - git->first.length())  << formatMtime(git->second.first);
			index <<  std::setw(20) << std::to_string(git->second.second)<< std::endl;
		}
		index <<	"</pre><hr></body></html>";
		
		ws::fileHandler::write(filePath,index.str());
	}

	std::string	Response::formatMtime(struct stat sta)
	{
		std::string months [] = {"Jan", "Feb", "MAar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Ded"};
		std::stringstream date;
		this->fileStat = sta;
		struct tm * tm;

		tm = gmtime(&this->fileStat.st_mtime);
		date << std::setw(2) << std::setfill('0') << tm->tm_mday ;
		date << "-" << months[tm->tm_mon].c_str();
		date << "-" <<tm->tm_year+1900 <<" " ;
		date << std::setw(2) << std::setfill('0') <<(tm->tm_hour+1)%24 <<":"<< std::setw(2) << std::setfill('0') <<tm->tm_min;
		return date.str();
	}
	
	void	Response::craftPostResponse(Request &request)
	{
		std::string absoluteResourcePath = buildAbsolutePath(request);

		if(isCgi())
		{
			checkCgi(absoluteResourcePath,request);
		}
		else if(isUpload())
		{
			int endPos = request.getUri().length();
			--endPos;
			if((request.getUri().at(endPos) == '/') || (ws::fileHandler::checkIfExist(absoluteResourcePath) && isDir(absoluteResourcePath)))
			{
				this->statusCode = "500";
				buildResponse(request);
				throw "Internal server error";
			}

			absoluteResourcePath = this->currentLocation.getUploadPath() + request.getUri().replace(0,this->currentLocation.getLocation_match().length(),"");
			
			if(ws::fileHandler::checkIfExist(absoluteResourcePath))
			{
				this->statusCode = "409";
				buildResponse(request);
				throw "Cant upload this resource already exist";
			}
			else
			{
				std::string tmpFile = request.getFilePath();
				
				int ret = 1;
				size_t pos = absoluteResourcePath.find_last_of('/');
				std::string dirs = absoluteResourcePath.substr(0,pos);
				int pathStat = stat(dirs.c_str(), &this->fileStat);

				if( pathStat != 0)
				{	
					int flag = 0;

					for (size_t i = 0; i < dirs.length(); i++)
					{
						if(dirs[i] == '/')
						{
							dirs[i] = '\0';
							if(mkdir(dirs.c_str(),0777) != 0)
								flag = 1;
							else
								flag = 0;
							dirs[i] = '/';
						}
					}
					if(mkdir(dirs.c_str(),0777) != 0)
						flag = 1;
					else
						flag = 0;
					if(flag)
						ret =  500;
				}
				if(pathStat == 0)
				{
					if( this->fileStat.st_mode & S_IFDIR )
					{	
						if(this->fileStat.st_mode & S_IWUSR)
							ret =  1;
						else if (!(this->fileStat.st_mode & S_IWUSR))
							ret = 500;
					}
					else
						ret =  500;
				}

				if(ret == 1)
				{
					int err = rename(tmpFile.c_str(),absoluteResourcePath.c_str());
					if(err)
					{	
						this->statusCode = "500";
						buildResponse(request);
						throw "Internal server error rename failed";
					}
					else
					{
						this->statusCode = "201";
						this->bodyPath.clear();
						setHeader("Content-Length","0");
						this->response_is_tmp = false;
					}
				}
				else 
				{
					this->statusCode = "500";
					buildResponse(request);
					throw "Internal server error";
				}
			}
		}
		else
		{
			this->statusCode = "403";
			buildResponse(request);
			throw "Coudldnt process the upload";
		}
	}
	

	void	Response::searchForLocation(Request &request)
	{
		Server s = request.getServer();
		std::vector<Location> locs = s.getLocation();
		size_t locationLength = 0;
		std::vector<Location>::iterator it;
		for (it = locs.begin(); it != locs.end() ;++it)
		{
			Location  l1= *it;
			if(locationLength <= l1.getLocation_match().length() && strncasecmp(l1.getLocation_match().c_str(), request.getUri().c_str(), l1.getLocation_match().length()) == 0)
			{	
				locationLength = l1.getLocation_match().length();
				this->currentLocation = *it;
			}
		}
	}

	std::vector<std::string> Response::pathSpliter(std::string &filePath)
	{
		std::vector<std::string> dirList;
		std::string path = filePath;
		char* token = strtok(const_cast<char*>(path.c_str()),"/");
		
		while (token != NULL)
		{	
			dirList.push_back(std::string(token));
			token = strtok (NULL, "/");			
		}
		return dirList;
	}

	void Response::checkCgi(std::string &resourcePath, Request &request)
	{
		std::string filePath = resourcePath;
		if(ws::fileHandler::checkIfExist(resourcePath))
		{
			if(isDir(resourcePath))
			{
				if(isIndexes())
				{
					std::vector<std::string> indexList = getIndexes();
					for (size_t i = 0; i < indexList.size(); i++)
					{
						std::string indexPath = buildAbsolutePath(request)+indexList[i];
						if(ws::fileHandler::checkIfExist(indexPath))
							if(isFile(indexPath))
								if(isCgi())
									filePath = indexPath;
					}
				}
				else
				{
					std::string defaultIndexPath = buildAbsolutePath(request)+"index.html";
					if(ws::fileHandler::checkIfExist(defaultIndexPath))
						filePath = defaultIndexPath;
				}
			}
		}
		request.cgi_ptr = new CGI();
		request.cgi_ptr->cgi(request, getCgiPath().c_str(), filePath.c_str());
		throw "Calling cgi";
	}

	std::string Response::buildLocationPath(std::string &path, Request &request)
	{
		std::string locationPath;
		if(strncasecmp(path.c_str(),"http://",7) == 0)
		{
			locationPath = path;
		}
		else
			locationPath = "http://"+getHost(request) + ":"+getPort(request) +"/"+path; 
		return locationPath;
	}

	void Response::setDateHeader()
	{
		// Mon, 06 Jun 2022 03:48:42 GMT
		time_t curr_time;
		struct tm * tm;
		std::string days[] =  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
		std::string months [] = {"Jan", "Feb", "MAar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Ded"};
		time(&curr_time);
		tm = gmtime(&curr_time);
		std::stringstream date;
		date << days[tm->tm_wday].c_str() <<", " << std::setw(2) << std::setfill('0') << tm->tm_mday ;
		date << " " << months[tm->tm_mon].c_str() << " " << tm->tm_year+1900 <<" " ;
		date << std::setw(2) << std::setfill('0') <<(tm->tm_hour+1)%24 <<":"<< std::setw(2) << std::setfill('0') <<tm->tm_min <<":"<< std::setw(2) << std::setfill('0') << tm->tm_sec << " GMT+1";
		this->headers_list.insert(std::make_pair("Date", date.str()));
	}

	std::pair<std::string, bool> Response::getbody()
	{
		return std::pair<std::string,bool>(this->bodyPath,this->response_is_tmp);
	}

	long long Response::getFileSize(std::string &filePath)
	{
		if(!filePath.empty() && filePath.length() > 0)
		{
			stat(filePath.c_str(), &this->fileStat);
			return this->fileStat.st_size;
		}
		return 0;
	}

	std::string Response::getMessage(std::string &statusCode)
	{
		if(Config::statusCodeMessages.find(statusCode) != Config::statusCodeMessages.end())
			return Config::statusCodeMessages.find(statusCode)->second;
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

	std::pair<int,std::string>	Response::getRedirection()
	{
		return std::make_pair<int , std::string>(this->currentLocation.getRedirectUri().begin()->first,this->currentLocation.getRedirectUri().begin()->second);
	}

	std::string	Response::getCgiPath()
	{
		return (this->currentLocation.getCgiPath());
	}

	std::string	Response::getHost(const Request &request)
	{
		return request.getServer().getIp();
	}

	std::string	Response::getPort(const Request &request)
	{
		return std::to_string(request.getHostPort());
	}

	void Response::setContentLength(std::string filePath)
	{
		if(!filePath.empty() && filePath.length() > 0)
			setHeader("Content-Length",std::to_string(getFileSize(filePath)));
	}

	void Response::setHeader(std::string key, std::string value)
	{
		this->headers_list[key] = value;
	}

	void Response::setContentType(std::string &filePath)
	{
		int pos = filePath.find_last_of('.');
		pos++;
		std::string extention = filePath.substr(pos,filePath.size()-pos);
		if(Config::mimetypeMap.find(extention) != Config::mimetypeMap.end())
			setHeader("Content-Type",Config::mimetypeMap[extention]);
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
		if(stat(path.c_str(),&this->fileStat) < 0)    
			return false;

		if(permission == "r")
			return (this->fileStat.st_mode & S_IRUSR);
		if(permission == "w")
			return (this->fileStat.st_mode & S_IWUSR);
		if(permission == "x")
			return (this->fileStat.st_mode & S_IXUSR);
		
		return false;
	}

	bool Response::isDir(std::string &resourcePath)
	{
		stat( resourcePath.c_str(), &this->fileStat );
		return (this->fileStat.st_mode & S_IFDIR);
	}
	
	bool Response::isFile(std::string &resourcePath)
	{
		stat( resourcePath.c_str(), &this->fileStat );
		return (this->fileStat.st_mode & S_IFREG);
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

	bool Response::hasUpload()
	{
		return (!this->currentLocation.getUploadPath().empty());
	}

	void Response::isResourceValid(std::string &resourcePath, Request &request)
	{
		if(!ws::fileHandler::checkIfExist(resourcePath))
		{
			this->statusCode = "404";
			buildResponse(request);
			throw "Resource doesnt exist";
		}
	}

	bool Response::isCgi()
	{
		return (!this->currentLocation.getCgiPath().empty());
	}

	bool Response::isUpload()
	{
		return (!this->currentLocation.getUploadPath().empty());
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

	void Response::isResourceEndSlash1(Request &request)
	{
		int endPos = request.getUri().length();
		--endPos;
		if(request.getUri().at(endPos) != '/')
		{
			this->statusCode = "409";
			buildResponse(request);
			throw "Conflict";
		}
	}
	
	void Response::craftDeleteResponse(Request &request) 
	{
		std::string absoluteResourcePath = buildAbsolutePath(request);
		try
		{
			isResourceValid(absoluteResourcePath,request);
		}
		catch(const char* msg)
		{
			throw msg;
		}
		if(isDir(absoluteResourcePath))
		{
			if(!isPermission(absoluteResourcePath, "x"))
			{
				this->statusCode = "403";
				buildResponse(request);
				throw "Resource have no permissions";
			}
			isResourceEndSlash1(request);
			searchForLocation(request);
			if(isCgi())
			{
				checkCgi(absoluteResourcePath,request);
			}
			else
			{
				if (!remove_directory(absoluteResourcePath.c_str()))
				{
					this->statusCode = "204";
					this->bodyPath.clear();
					throw "Deleted successfully";
				}
				else
				{
					if(!isPermission(absoluteResourcePath, "x") || !isPermission(absoluteResourcePath, "w"))
					{
						this->statusCode = "403";
						buildResponse(request);
						throw "Resource have no permissions";
					}
					else
					{
						this->statusCode = "500";
						buildResponse(request);
						throw "Internal server error remove failed";
					}
					
				}
				
			}
		}
		else if(isFile(absoluteResourcePath))
		{
			if(isCgi())
			{
				request.cgi_ptr = new CGI();
				request.cgi_ptr->cgi(request, getCgiPath().c_str(), absoluteResourcePath.c_str());
				throw "Calling cgi";
			}
			else
			{
				if(fileHandler::removeFile(absoluteResourcePath) == 0)
				{
					this->statusCode = "204";
					this->bodyPath.clear();
					throw "Deleted successfully";
				}
				else
				{
					this->statusCode = "500";
					buildResponse(request);
					throw "Internal server error remove failed";
				}
			}

		}
	}
	
	int Response::remove_directory(const char *path) {
		DIR *d = opendir(path);
		size_t path_len = strlen(path);
		int r = -1;

		if (d) {
			struct dirent *p;

			r = 0;
			while (!r && (p=readdir(d))) {
				int r2 = -1;
				char *buf;
				size_t len;

				/* Skip the names "." and ".." as we don't want to recurse on them. */
				if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
					continue;

				len = path_len + strlen(p->d_name) + 2; 
				buf = (char *)malloc(len);

				if (buf) {
					struct stat statbuf;

					snprintf(buf, len, "%s/%s", path, p->d_name);
					if (!stat(buf, &statbuf)) {
						if (S_ISDIR(statbuf.st_mode))
						r2 = remove_directory(buf);
						else
						r2 = unlink(buf);
					}
					free(buf);
				}
				r = r2;
			}
			closedir(d);
		}

		if (!r)
			r =rmdir(path);
		
  		return r;
	}
}