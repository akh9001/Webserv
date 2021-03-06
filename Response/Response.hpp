/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:56 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/28 12:58:56 by laafilal         ###   ########.fr       */
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
			std::string 						bodyPath;
			std::string 						statusCode;
			Location 							currentLocation;
			std::map<std::string, std::string>	headers_list;

		private:
			//response builders
			std::string 				headerBuilder();
			void						buildResponse(Request &request);
			void						bodyDefaultTemplate(std::string &responsePath);
			void						autoIndexTemplate(std::multimap<std::string, std::pair<struct stat , long long> > &dirList, std::string filePath);
			void						craftGetResponse(Request &request);
			void						craftPostResponse(Request &request);
			void 						craftDeleteResponse(Request &request);
			void 						checkIndexes(Request &request);
			void						checkCgi(std::string &filepath, Request &request);
			void						autoIndexHandler(Request &request);
			void						checkDefaultIndex(std::string &absoluteResourcePath, Request &request);
			void 						checkResourceLocation(Request &request);
			void 						checkResource(Request &request);
			void 						checkRedirection(Request &request);
			void 						checkAllowedMethods(Request &request);
			void 						defineMethode(Request &request);
			void						checkRoot(Request &request);

			//helpers
			std::string 				buildPath(std::string &resourcePath);
			std::string 				buildAbsolutePath(Request &request);
			void						searchForLocation(Request &request);
			std::vector<std::string>	pathSpliter(std::string &filePath);
			int							directoriesHandler(std::string filename, std::vector<std::string> dirList, int i, std::string originPath);
			std::string					formatMtime(struct stat);
			int							remove_directory(const char *path);
			std::string 				buildLocationPath(std::string &path,  Request &Request);

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
			std::string					getHost(const Request &request);
			std::string					getPort(const Request &request);


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
			void 						isResourceValid(std::string &resourcePath, Request &request);
			void 						isResourceEndSlash(Request &request);
			void 						isResourceEndSlash1(Request &request);
			bool 						isCgi();
			bool						isUpload();

	};
	std::string 								ltrim(const std::string &s);
	std::string 								rtrim(const std::string &s);
	std::string 								trim(const std::string &s);
}
#endif