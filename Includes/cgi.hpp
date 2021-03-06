/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:32 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/30 15:13:23 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP
#include "networking.hpp"
#include "../Parsing/Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Includes/client.hpp"

class 	Request;
class 	Response;
struct	Client;

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
		int		get_pid();
		void	set_env_map(const Request &request, const char *script_path);
		char	**set_envp(const std::vector<std::string> &cookies);
		int		cgi(const Request &request, const char *cgi_path, const char *script_path);
		bool	execute(char **args, const Request &request);
		bool	is_finished(Client &client);
		void	craft_response(Client &client);
		std::string getDateHeader();
		~CGI(void);
};

#endif