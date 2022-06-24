/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:00 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/24 16:11:38 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cgi.hpp"
#include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <iostream>
// #include <string>

// _env["DOCUMENT_ROOT"] = "/Users/akhalidy/Desktop/akh/";
// _env["HTTP_HOST"] = request.getHostIp();
// _env["REMOTE_ADDR"] = "127.0.0.1";
// _env["REMOTE_HOST"] = "127.0.0.1";
// _env["DOCUMENT_URI"] = script_path;
// _env["REMOTE_PORT"] = "8001";//std::to_string(request.getHostPort());
// _env["PATH"] = std::string(std::getenv("PATH"));
	// _env["SCRIPT_NAME"] = "/Users/akhalidy/Desktop/akh/cgi-bin/php-cgi";

CGI::CGI(void) : finished(false)
{
	char	filename[] = "/tmp/tmp_cgi_XXXXXX";
	file = mktemp(filename);
	std::cout << file << std::endl;
}

void	CGI::set_env_map(const Request &request, const char *script_path)
{

	std::string		method = request.getMethod();

	if(request.getContentLenth() && method != "GET")
	{
		_env["CONTENT_LENGTH"] = request.getHeaderMap()["Content-Length"];
		_env["CONTENT_TYPE"] = request.getHeaderMap()["Content-Type"];
	}
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["QUERY_STRING"] = request.getQuery();
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["SCRIPT_FILENAME"] = script_path; //* script_path is the path of the file to be executed.
	_env["SERVER_SOFTWARE"] = "WEBSERV";
	_env["SERVER_PROTOCOL"] = request.getVersion();
	_env["REDIRECT_STATUS"] =  "true";
}

char**	CGI::set_envp(void)
{
	char											**envp;
	int												i = 0;
	int												len = _env.size();
	std::string										str;
	std::map<std::string, std::string>::iterator	it	= _env.begin();

	envp = new char* [len + 1];
	while (it != _env.end())
	{
		str = it->first + "=" + it->second;
		envp[i] = strdup(str.c_str());
		i++;
		it++;
	}
	envp[i] = NULL;
	return (envp);
}

bool	CGI::execute(char **args, const Request &request)
{
	int			out;
	int			in = 0;
	int			status;
	int			ret;
	const char	*post_body = request.getFilePath().c_str();
	std::cout << "Fuuuuuuuu !" << file << std::endl;
	_pid = fork();
	if (_pid == -1)
	{
		std::cerr << "fork failed !" << std::endl;
		return false;
	}
	if (_pid == 0)
	{
		if (post_body && *post_body)
			in = open(post_body, O_RDONLY);
		out = open(file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (in)
		{
			dup2(in, 0);
			close(in);
		}
		dup2(out, 1);
		set_env_map(request, args[1]);
		if (execve(args[0], args, set_envp()) == -1)
		{
			std::cerr << "execvp failed !" << std::endl;
			exit(111);
		}
	}
	return true;
}

int	CGI::cgi(const Request &request, const char *cgi_path, const char *script_path)
{
	char			*args[3];

	args[0] = (char *)cgi_path;
	args[1] = (char *)script_path;
	args[2] = NULL;
	std::cerr << GREEN << "MAKE IT HERE !" << RESET << std::endl;
	if (execute(args, request))
		return true;
	return false;
}

//TODO 
//* When execve failed I should return a specific status.
//* I should separate the query args