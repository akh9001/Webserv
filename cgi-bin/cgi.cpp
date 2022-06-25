/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:00 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/24 23:46:47 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cgi.hpp"
#include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <iostream>
#include <sstream>

CGI::CGI(void) : finished(false) {
  char filename[] = "/tmp/tmp_cgi_XXXXXX";
  file = mktemp(filename);
  //*   std::cout << file << std::endl;
}

void CGI::set_env_map(const Request &request, const char *script_path) {

  std::string method = request.getMethod();

  if (request.getContentLenth() && method != "GET") {
    _env["CONTENT_LENGTH"] = request.getHeaderMap()["Content-Length"];
    _env["CONTENT_TYPE"] = request.getHeaderMap()["Content-Type"];
  }
  _env["GATEWAY_INTERFACE"] = "CGI/1.1";
  _env["QUERY_STRING"] = request.getQuery();
  _env["REQUEST_METHOD"] = request.getMethod();
  _env["SCRIPT_FILENAME"] =
  script_path; //* script_path is the path of the file to be executed.
  _env["SERVER_SOFTWARE"] = "WEBSERV";
  _env["SERVER_PROTOCOL"] = request.getVersion();
  _env["REDIRECT_STATUS"] = "true";
}

char **CGI::set_envp(void) {
  char **envp;
  int i = 0;
  int len = _env.size();
  std::string str;
  std::map<std::string, std::string>::iterator it = _env.begin();

  envp = new char *[len + 1];
  while (it != _env.end()) {
    str = it->first + "=" + it->second;
    envp[i] = strdup(str.c_str());
    i++;
    it++;
  }
  envp[i] = NULL;
  return (envp);
}

bool CGI::execute(char **args, const Request &request) {
  int out;
  int in = 0;
  int status;
  int ret;
  const char *post_body = request.getFilePath().c_str();
  _pid = fork();
  if (_pid == -1) {
    std::cerr << "fork failed !" << std::endl;
    _pid = 1;
    return false;
  }
  if (_pid == 0) {
    if (post_body && *post_body)
      in = open(post_body, O_RDONLY);
    out = open(file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (in) {
      dup2(in, 0);
      close(in);
    }
    dup2(out, 1);
    set_env_map(request, args[1]);
    if (execve(args[0], args, set_envp()) == -1) {
      std::cerr << "execve failed !" << std::endl;
      exit(111);
    }
  }
  return true;
}

int CGI::cgi(const Request &request, const char *cgi_path,
             const char *script_path) {
  char *args[3];

  args[0] = (char *)cgi_path;
  args[1] = (char *)script_path;
  args[2] = NULL;
  if (execute(args, request))
    return true;
  return false;
}

void cgi_internal_error(Client &client) {
  ws::Response response;

  delete client.request.cgi_ptr;
  client.request.cgi_ptr = NULL;

  Location location = client.request.getLocation();
  std::string status = "500";
  client.buffer = response.getHeaders(client.request, location, status);
  client.body_inf = response.getbody();
  if (client.body_inf.first.size() > 0)
    client.file.open(client.body_inf.first);
}

bool CGI::is_finished(Client &client) {
  int pid;
  int status;
  if (finished)
    return true;
  pid = waitpid(_pid, &status, WNOHANG);
  if (pid == 0)
    return false;
  if (pid == -1 || !WIFEXITED(status) || WEXITSTATUS(status) == 111) {
    cgi_internal_error(client);
    return true;
  }

  finished = true;
  // call a function that will open the tmp file as client.file and read all the
  // headers and craft a header response and put it in the
  //  header response and put it in the client.buffer
  //   std::cerr << "Crafting response\n";
  craft_response(client);
  return true;
}

void CGI::craft_response(Client &client) {
  client.buffer = "Server: WebServ/1.0\r\n" + getDateHeader();
  _status = "200";
  client.file.open(file);

  for (std::string line; std::getline(client.file, line);) {
    if (line.empty() || line == "\r")
      break;
    if (strncasecmp("Status:", line.c_str(), 7) == 0) {
      std::string::iterator it = line.begin() + 7;
      while (*it == ' ')
        ++it;
      _status = std::string(it, std::find(it, line.end(), ' '));
    } else {
      client.buffer += line + "\r\n";
    }
  }
  struct stat st;
  if (stat(this->file.c_str(), &st) == -1)
    return cgi_internal_error(client);
//*   std::cerr << st.st_size - client.file.tellg() << std::endl;
  std::stringstream header;
  header << "HTTP/1.1 " << _status << " " << ws::Response::getMessage(_status)
         << "\r\n";
  header << client.buffer;
  header << "Content-Length: " << st.st_size - client.file.tellg() << "\r\n";
  header << "\r\n";
  client.buffer = header.str();
}

std::string CGI::getDateHeader() {
  time_t curr_time;
  struct tm *tm;
  std::string days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  std::string months[] = {"Jan", "Feb", "MAar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep",  "Oct", "Nov", "Ded"};
  // Mon, 06 Jun 2022 03:48:42 GMT
  time(&curr_time);
  tm = gmtime(&curr_time);
  std::stringstream date;
  date << days[tm->tm_wday].c_str() << ", " << std::setw(2) << std::setfill('0')
       << tm->tm_mday;
  date << " " << months[tm->tm_mon].c_str() << " " << tm->tm_year + 1900 << " ";
  date << std::setw(2) << std::setfill('0') << (tm->tm_hour + 1) % 24 << ":"
       << std::setw(2) << std::setfill('0') << tm->tm_min << ":" << std::setw(2)
       << std::setfill('0') << tm->tm_sec << " GMT+1";
  return ("Date: " + date.str() + "\r\n");
}

// TODO
//* When execve failed I should return a specific status.
//* I should separate the query args