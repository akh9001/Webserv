/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:56 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/14 10:39:16 by laafilal         ###   ########.fr       */
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
			std::string getHeaders(Request &request,Location &location, Config &config, std::string &statusCode);
			std::pair<std::string, bool> getbody();
			
		private:
			int buildResponseTry;
			std::string bodyPath;
			std::string statusCode;
			std::map<std::string, std::string> headers_list;
			Location currentLocation;

		private:
			std::string headerBuilder();
			void	buildResponse(Request &request);
			void	bodyDefaultTemplate(std::string &responsePath);

			void setDateHeader();
			void setContentLength(std::string filePath);
			void setHeader(std::string key, std::string value);

			long long getFileSize(std::string &filePath);
			std::string getMessage(std::string &statusCode);
			std::string getErrorPage();

			bool isMethodeAllowed(Request &request);
			bool isErrorPage();

	};
	
	static std::map<std::string,std::string> statusCodeMessages;
	void init_statusCodeMessages();
}

#endif