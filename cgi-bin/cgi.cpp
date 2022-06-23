/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:00 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/23 13:03:52 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cgi.hpp"
#include <algorithm>
#include <ostream>
#include <string>

bool	execute(const char *cmd, char **args, const char *post_body)
{
	int		pid;
	int		out;
	int		in = 0;
	int		fd[2];
	int		status;

	out = open("/tmp/file", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (post_body && *post_body)
		in = open(post_body, O_RDONLY);
	pid = fork();
	if (pid == -1)
		std::cerr << "fork failed !" << std::endl;
	if (pid == 0)
	{
		if (in)
		{
			dup2(in, 0);
			close(in);
		}
		dup2(out, 1);
		close(fd[0]);
		close(fd[1]);
		if (execvp(cmd, args) == -1)
		{
			std::cerr << "execvp failed !" << std::endl;
			exit(111);
		}
	}
	wait(&status);
	if (in)
		close(in);
	close(out);
	close(fd[0]);
	close(fd[1]);
	if (WIFEXITED(status))
		if (WEXITSTATUS(status) == 111)
			return(false);
	return true;
}

char	**envp(std::map<std::string, std::string> env)
{
	char											**envp;
	int												i = 0;
	int												len = env.size();
	std::string										str;
	std::map<std::string, std::string>::iterator	it	= env.begin();

	envp = (char **)malloc(sizeof(char *) * (len + 1));
	while (i < len)
	{
		str = it->first + "=" + it->second;
		envp[i] = strdup(str.c_str());
		i++;
		it++;
	}
	envp[i] = NULL;
	return (envp);
}

void	set_envp(Request request, const char *script_path)
{

	if(request.getContentLenth())
	{
		setenv("CONTENT_LENGTH", request.getHeaderMap()["Content-Length"].c_str(), 1);
		setenv("CONTENT_TYPE", request.getHeaderMap()["Content-Type"].c_str(), 1);
	}
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", request.getQuery().c_str(), 1);
	setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
	setenv("SCRIPT_FILENAME", script_path, 1); //* script_path is the path of the file to be executed.
	setenv("SERVER_SOFTWARE", "WEBSERV", 1);
	setenv("SERVER_PROTOCOL", request.getVersion().c_str(), 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

size_t	count_occurence(const std::string &str)
{
	size_t	found = 0;
	size_t	i = 0;

	while ((found = str.find("&", found)) != std::string::npos)
	{
		i++;
		found++;
	}
	return i;
}

int	cgi(const Request &request, const char *cgi_path, const char *script_path)
{
	char			*args[3];

	set_envp(request, script_path);
	args[0] = (char *)cgi_path;
	args[1] = (char *)script_path;
	args[2] = NULL;
	if (execute(args[0], args, request.getFilePath().c_str()))
		return true;
	return false;
}
//TODO 
//* When execve failed I should return a specific status.
//* I should separate the query args