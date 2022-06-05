/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 03:03:21 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/05 01:34:47 by laafilal         ###   ########.fr       */
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
			std::string resPath;
			std::string resourcePath;
			int buildResponseTry;
		public:
			bool response_ready;
			bool response_is_tmp;
			// bool building_response;
			// std::string resBuffer;
		
		public:
			Response();
			~Response();
			// const char* getResponse(Request req , Location loc);
			std::string getResponse(ws::Req req , ws::Location loc, int statusCode);
			void buildResponse(std::string statusCode, ws::Req req , ws::Location loc);
			void craftResponse(ws::Req req , ws::Location loc);
			void craftGetPostRequests(ws::Req req , ws::Location loc);
			void craftDeleteRequests(ws::Req req , ws::Location loc);
			void defineResourceType(ws::Req req , ws::Location loc);
			void cgiChecker(ws::Req req , ws::Location loc);
			void cgiHandler(ws::Req req , ws::Location loc);
			void checkIndex(ws::Req req , ws::Location loc);
			void checkAutoIndex(ws::Req req , ws::Location loc);
			std::string builPath(ws::Location loc, std::string resourcePath);
			//checkers
			bool methodeAllowed(ws::Req req , ws::Location loc);
			void checkRedirections(ws::Req req , ws::Location loc);
			bool isRedirection(ws::Location loc);
			bool isResourceExist(ws::Req req , ws::Location loc);
			bool isResourceDir(ws::Req req , ws::Location loc);
			bool isResourceFile(ws::Req req , ws::Location loc);
			bool isCGI(ws::Location loc);
			bool isUriValid(ws::Req req , ws::Location loc);
			bool isIndex(ws::Location loc);
			bool isErrorPages(ws::Location loc);
			bool isFilePermission(std::string path, std::string permission);

			//getters
			std::string getResourcePath(ws::Req req, ws::Location loc);
			std::string getMethode(ws::Req req);
			std::string getAutoIndex(ws::Location loc);
			std::string getErrorPage(ws::Location loc, std::string statusCode);



			
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