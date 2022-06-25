/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:56 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/24 22:58:15 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "../FileHandler/fileHandler.hpp"
# include "../Parsing/Request/Request.hpp"
# include "../Parsing/Config/Config.hpp"
# include "../Parsing/Config/Server.hpp"
# include "../Parsing/Config/Location.hpp"
#include "../Includes/cgi.hpp"
# include <map>

# include <iomanip> // only for linux
class CGI;
class Request;
namespace ws {
	
	class Response
	{
		public:
			bool response_is_tmp;
			
			
		public:
			Response();
			~Response();
			std::string 					getHeaders(Request &request,Location &location, std::string &statusCode);
			std::pair<std::string, bool> 	getbody();
			static std::string 			getMessage(std::string &statusCode);

			
		private:
			struct stat 						fileStat;
			int 								buildResponseTry;
			std::string 						bodyPath;
			std::string 						statusCode;
			Location 							currentLocation;
			std::map<std::string, std::string>	headers_list;

		private:
			//response builders
			std::string 				headerBuilder();
			void						buildResponse();
			void						bodyDefaultTemplate(std::string &responsePath);
			void						autoIndexTemplate(std::multimap<std::string, std::pair<struct stat , long long> > &dirList, std::string filePath);
			void						craftGetRequests(Request &request);
			void						craftPostRequests(Request &request);
			void 						craftDeleteRequest(Request &request);
			void 						checkIndexes(Request &request);
			void						checkCgi(std::string &filepath, Request &request);
			void						autoIndexHandler(Request &request);
			void						checkDefaultIndex(std::string &absoluteResourcePath);
			void 						checkResourceLocation(Request &request);
			void 						checkResource(Request &request);
			void 						checkRedirection();
			void 						checkAllowedMethods(Request &request);
			void 						defineMethode(Request &request);
			void						checkRoot();

			//helpers
			std::string 				buildPath(std::string &resourcePath);
			std::string 				buildAbsolutePath(Request &request);
			void						searchForLocation(Request &request);
			std::vector<std::string>	pathSpliter(std::string &filePath);
			int							directoriesHandler(std::string filename, std::vector<std::string> dirList, int i, std::string originPath);
			std::string					formatMtime(struct stat);

			//setters
			void 						setDateHeader();
			void 						setContentLength(std::string filePath);
			void 						setHeader(std::string key, std::string value);
			void						setContentType(std::string &filePath);

			//getters
			long long 					getFileSize(std::string &filePath);
			std::string 				getErrorPage();
			std::string 				getMethod(Request &request);
			std::vector<std::string> 	getIndexes();
			std::pair<int,std::string>	getRedirection();
			std::string					getCgiPath();

			//checkers
			bool 						isMethodeAllowed(Request &request);
			bool 						isErrorPage();
			bool 						isPermission(std::string &path, std::string permission);
			bool 						isDir(std::string &resourcePath);
			bool 						isFile(std::string &resourcePath);
			bool 						isRedirection();
			bool 						isIndexes();
			bool 						isAutoIndexOn();
			bool 						hasUpload();
			void 						isResourceValid(std::string &resourcePath);
			void 						isResourceEndSlash(Request &request);
			void 						isResourceEndSlash1(Request &request);
			bool 						isCgi();
			bool						isUpload();

	};
	
	static std::map<std::string,std::string>	statusCodeMessages;
	static std::map<std::string, std::string> 	mimetypeMap;
	void 										init_statusCodeMessages();
	void										init_mimetype();
	
	std::string 								ltrim(const std::string &s);
	std::string 								rtrim(const std::string &s);
	std::string 								trim(const std::string &s);
	int 										remove_directory(const char *path);
}
#endif