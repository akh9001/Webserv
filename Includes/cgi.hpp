/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:32 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/24 17:22:39 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP
#include "networking.hpp"
#include "../Parsing/Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Includes/client.hpp"
// #include <string>
// #include <strings.h>
// #include <sys/wait.h>

class Request;
class Response;
class Client;
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
		
		bool	is_finished(Client &client);
		
		void	craft_response(Client &client);


		std::string getDateHeader();
		// ~CGI(void);
};


#endif