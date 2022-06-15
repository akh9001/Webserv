/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:56 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/15 09:27:53 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "../FileHandler/fileHandler.hpp"
# include "../Parsing/Request/Request.hpp"
# include "../Parsing/Config/Config.hpp"
# include "../Parsing/Config/Server.hpp"
# include "../Parsing/Config/Location.hpp"
// # include "../utility/utility.hpp"
# include <map>
# include <iomanip> // only for linux

namespace ws {
	
	class Response
	{
		public:
			bool response_is_tmp;
			
		public:
			Response();
			~Response();
			std::string getHeaders(Request &request,Location &location, std::string &statusCode);
			std::pair<std::string, bool> getbody();
			
		private:
			int buildResponseTry;
			std::string bodyPath;
			std::string statusCode;
			std::string headers;
			std::map<std::string, std::string> headers_list;
			Location currentLocation;

		private:
			//response builders
			std::string headerBuilder();
			void	buildResponse(Request &request);
			void	bodyDefaultTemplate(std::string &responsePath);

			//routing
			void checkResource(Request &request);
			void defineMethode(Request &request);

			//helpers
			std::string builPath(std::string &resourcePath);

			//setters
			void setDateHeader();
			void setContentLength(std::string filePath);
			void setHeader(std::string key, std::string value);

			//getters
			long long getFileSize(std::string &filePath);
			std::string getMessage(std::string &statusCode);
			std::string getErrorPage();
			std::string getMethod(Request &request);

			//checkers
			bool isMethodeAllowed(Request &request);
			bool isErrorPage();
			bool isPermission(std::string &path, std::string permission);
			bool isDir(std::string &resourcePath);
			bool isFile(std::string &resourcePath);
			bool isRedirection();

	};
	
	static std::map<std::string,std::string> statusCodeMessages;
	void init_statusCodeMessages();
	std::string ltrim(const std::string &s);
	std::string rtrim(const std::string &s);
	std::string trim(const std::string &s);
}
#endif