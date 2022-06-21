/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:00 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/21 13:00:43 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cgi.hpp"

void	execute(const char *cmd, char **args, const char *post_body)
{
	int		pid;
	int		out;
	int		in = 0;
	int		fd[2];

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
			perror("execve failllllll !!!!");
			exit(1);
		}
	}
	wait(NULL);
	if (in)
		close(in);
	close(out);
	close(fd[0]);
	close(fd[1]);
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

char	**args(const char *cmd, const char * args) // cmd is the path of the file to execute & args is the file.extension(.php/ .py) to be executed
{
	char	**args_;
	
	args_ = (char **)malloc(sizeof(char *) * 3);
	args_[0] = strdup(cmd);
	args_[1] = strdup(args);
	args_[2] = NULL;
	return (args_);
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

int	cgi(const Request &request, const char *script_path)
{
	char			**args_;
	std::string		cgi_path;
	
	cgi_path = std::string(getcwd(NULL, 0))+ "/cgi-bin/php-cgi";
	args_ = args(cgi_path.c_str(), script_path);
	set_envp(request, script_path);
	execute(args_[0], args_, request.getFilePath().c_str());
	return 0;
}
