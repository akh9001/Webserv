/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:32 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/24 17:02:16 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "networking.hpp"
#include "../Parsing/Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Includes/client.hpp"
#include <string>
#include <strings.h>
#include <sys/wait.h>

// extern char **environ;
// int	cgi(const Request &request, const char *cgi_path, const char *script_path);
class CGI
{
	private :
		std::map<std::string, std::string>	_env;
		std::string							file;
		int									_pid;
		std::string							_status;
		bool								finished;
	public:
		CGI(void);
		void	set_env_map(const Request &request, const char *script_path);
		char	**set_envp(void);
		int		cgi(const Request &request, const char *cgi_path, const char *script_path);
		bool	execute(char **args, const Request &request);
		
		bool	is_finished(Client &client)
		{
			int pid;
			int	status;
			
			if (finished)
				return true;
			pid = waitpid(_pid, &status, WNOHANG);
			if (pid == -1 || !WIFEXITED(status) || WEXITSTATUS(status) == 111) {
				client.request.cgi_ptr = NULL;
				delete this;
				
				ws::Response response;
		
				Location location = client.request.getLocation();
				
				client.buffer = response.getHeaders(client.request, location ,status);
				client.body_inf = response.getbody();
				return true;
			}
			if (pid == 0)
				return false;
			finished = true;
			// call a function that will open the tmp file as client.file and read all the headers and craft a header response and put it in the 
			//  header response and put it in the client.buffer
			
			return true;
		}
		
		void	craft_response(Client &client)
		{
			client.buffer = "Server: WebServ/1.0\r\n" + getDateHeader();
			_status = "200";
			client.file.open(file);
			
			for (std::string line; std::getline(client.file, line); ) 
			{
				if (line.empty() || line == "\r")
					break;
				if (strncasecmp("Status:", line.c_str(), 7) == 0) {
					std::string::iterator it = line.begin() + 7;
					while (*it == ' ') ++it;
					_status = std::string(it, std::find(it, line.end(), ' '));
				}
				else {
					client.buffer += line + "\r\n";
				}
			}
			client.buffer = "HTTP/1.1 "+ _status +" "+ ws::Response::getMessage(_status)+ "\r\n" + client.buffer + "\r\n";
		}


		std::string getDateHeader()
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
			date << std::setw(2) << std::setfill('0') <<(tm->tm_hour+1)%24 <<":"<< std::setw(2) << std::setfill('0') <<tm->tm_min <<":"<< std::setw(2) << std::setfill('0') << tm->tm_sec << " GMT+1";
			return ("Date: " + date.str() + "\r\n");
		}
		// ~CGI(void);
};
