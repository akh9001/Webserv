/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 03:03:21 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/11 02:36:50 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "../helpers/fileHandler.hpp"
# include <map>


namespace ws {

	static std::map<std::string,std::string> statusCodeMessages;

	void init_statusCodeMessages();
	

	class Req;
	class Location;
	class Response
	{
		private:
			std::string statusCodeResponseTemplate(std::string code, std::string message);
			// std::string resPath;
			std::string bodyPath;
			int buildResponseTry;
			std::string headers;
			std::string statusCode;
			std::map<std::string, std::string> headers_list;
		public:
			bool response_is_tmp;
			bool response_ready;

			// bool building_response;
		
		public:
			Response();
			~Response();
			// const char* getResponse(Request req , Location loc);
			// std::string getResponse(ws::Req req , ws::Location loc, int statusCode);
			// void buildResponse(std::string statusCode, ws::Req req , ws::Location loc);
			std::string getHeaders(ws::Req req , ws::Location loc, int statusCode);
			std::pair<std::string, bool> getbody();
			std::string headerBuilder();
			void buildResponse(std::string statusCode,ws::Req req , ws::Location loc);
			void checkResource(ws::Req req , ws::Location loc);
			// void craftGetPostRequests(ws::Req req , ws::Location loc);
			// void craftDeleteRequests(ws::Req req , ws::Location loc);
			void defineResourceType(ws::Req req , ws::Location loc);
			void cgiChecker(ws::Req req , ws::Location loc);
			void cgiHandler(ws::Req req , ws::Location loc);
			void checkIndex(ws::Req req , ws::Location loc);
			void checkAutoIndex(ws::Req req , ws::Location loc);
			std::string builPath(ws::Location loc, std::string resourcePath);
			void bodyDefaultTemplate(std::string statusCode, std::string resPath);
			std::vector<std::string> pathSpliter(ws::Location loc , std::string filePath);
			int directoriesHandler(ws::Location loc , std::string filename, std::vector<std::string> dirList, int i,std::string originPath);

			
			//checkers
			bool methodeAllowed(ws::Req req , ws::Location loc);
			void checkRedirections(ws::Req req , ws::Location loc);
			bool isRedirection(ws::Location loc);
			// bool isResourceExist(ws::Req req , ws::Location loc);
			bool isResourceDir(ws::Req req , ws::Location loc,std::string resourcePath);
			bool isResourceFile(ws::Req req , ws::Location loc);
			bool isCGI(ws::Location loc);
			bool isUriValid(ws::Req req , ws::Location loc);
			bool isIndex(ws::Location loc);
			bool isErrorPage(ws::Location loc,std::string statusCode);
			bool isFilePermission(std::string path, std::string permission);

			//getters
			
			std::string getResourcePath(ws::Req req, ws::Location loc);
			std::string getMethode(ws::Req req);
			std::string getAutoIndex(ws::Location loc);
			std::string getErrorPage(ws::Location loc, std::string statusCode);
			std::string getMessage(std::string statusCode);
			long long getFileSize(std::string filePath);
			// std::string getRedirectionStatus(ws::Location loc);
			std::string getRedirectionPath(ws::Location loc);
			std::string getLocationPath(ws::Location loc);
			std::string getUploadPath(ws::Location loc);
			std::string getUploadFileName(ws::Req req);
			std::string getRequestBodyPath(ws::Req req);

			//setters
			void setDateHeader();
			void setContentLength(std::string filePath);
			void setHeader(std::string key, std::string value);
			
			//asignment operator

			
			// bool defineResponseType(ws::Req req , ws::Location loc);
			// // std::string defineResponseType(ws::Req req , ws::Location loc);
			// bool craftGetPostResponse(ws::Req req , ws::Location loc);
			// bool craftDeleteResponse(ws::Req req , ws::Location loc);
			// void errorHandlingResponse(int statusCode);
			// std::string errorHandlingRespo(int statusCode);
			// //helpers
			// std::string getStatusMessage(std::string statusCode);
			// bool errorPageExist(std::string statusCode, ws::Location loc);
	};

}

#endif