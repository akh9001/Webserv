/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:32 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/24 13:17:17 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "networking.hpp"
#include "../Parsing/Request/Request.hpp"

// extern char **environ;
// int	cgi(const Request &request, const char *cgi_path, const char *script_path);

class CGI
{
	private :
		std::map<std::string, std::string>	_env;
		std::string							file;
		int									_pid;
		std::string							_status;
	public:
		CGI(void);
		void	set_env_map(const Request &request, const char *script_path);
		char	**set_envp(void);
		int		cgi(const Request &request, const char *cgi_path, const char *script_path);
		bool	execute(char **args, const Request &request);
		// ~CGI(void);
};