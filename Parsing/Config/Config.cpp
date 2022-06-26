/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 14:33:17 by mokhames          #+#    #+#             */
/*   Updated: 2022/06/26 12:45:36 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <map>


std::map<std::string,std::string>	Config::statusCodeMessages;
std::map<std::string, std::string> 	Config::mimetypeMap;

// ! //////////////////// Constuctors and destructor /////////////////

Config::Config(std::string file) : file_name(file) , full_file(), lines(), servers()
{
    init_statusCodeMessages();
	init_mimetype();
}

Config::~Config(){}

Config::Config(Config const& c) 
{
    *this = c;
}

Config &Config::operator=(Config const& c) 
{
    servers = c.servers;
    lines = c.lines;
    _binders = c._binders;
    init_statusCodeMessages();
	init_mimetype();
    return *this;
}

// ! /////////////////////// Getters & Setters //////////////////////
std::vector<Server> Config::getServers() const
{
    return servers;
}

std::string Config::getFullFile() const
{
    return full_file;
}

std::vector<std::string> Config::getLines() const
{
    return lines;
}
std::string Config::getLine(std::string &) const
{
    return "sever_name";
}

Server Config::getServer(int port, std::string &c) const
{
    std::vector<Server> a;
    int save = 0;
    int j = 0;
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (servers[i].getPort() == port)
            a.push_back(servers[i]);
    }
    // std::cout << "Server " << std::endl;
    if (!c.size() && a.size())
        return a[0];
    else if (!c.size())
            throw NoServerFoundException();
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i].checkServerName(c))
        {
            save = i;
            j++;
        }

    }
    if (j > 1)
        throw (MultpiteServerName());
    return servers[save];
}
 std::map<int, std::string> Config::getBinders() const
 {
        return _binders;
 }
void Config::setfile(std::string& file)
{
    full_file = file;
}
// ! /////////////////////// file_Readers //////////////////////


void Config::main_read()
{
    read_file();
    split_line();
    split_servers();
    main_error_check();
    parse_server();
    parse_bind_map();
        /* code */
    
}

void Config::read_file()
{
    std::fstream a(file_name);

    if (!a.is_open())
    {
        std::cout << "Error : " << file_name;
        throw fileErrorException();
    }
    std::string output;
    while (std::getline(a, output, '\t'))
        full_file += output;
}

void    Config::split_line()
{
    std::string line;
    std::string full_file1(full_file);
    size_t pos = 0;
    while ((pos = full_file1.find("\n")) != std::string::npos)
    {
        line = full_file1.substr(0, pos);
        if (line[0] != '\0')
        lines.push_back(line);
        full_file1.erase(0, pos + 1);
    }
    lines.push_back(line);
    full_file1.erase(0, pos + 1);
}

void Config::split_servers()
{
    int open = 1;
    Server  s1;
    std::vector<std::string>::iterator it = lines.begin();
    std::vector<std::string>::iterator save = it;
    while (it != lines.end())
    {
        if ((*it).find("server ") != std::string::npos && open == 1)
            open = 0;
        else if ((*it).find("server ") != std::string::npos && open == 0)
        {
            std::vector<std::string> a (save, it);
            s1.setServerline(a);
            servers.push_back(s1);
            save = it;
        }
        it++;
    }
    std::vector<std::string> a (save, it);
    s1.setServerline(a);
    servers.push_back(s1);
}

void Config::parse_server()
{
    for (size_t i = 0; i < servers.size(); i++)
        servers[i].parseLines();
    //  for (size_t i = 0; i < servers.size(); i++)
    // {
    //     //std::cout << "dadad" << std::endl;
    //     for (std::map<size_t, std::string>::iterator it = servers[i].getErrorPages().begin(); it != servers[i].getErrorPages().end(); it++)
    //     {
    //        std::cout << it->first << " " << it->second << std::endl;
    //     }
    // }
}

void Config::parse_bind_map()
{
    for (size_t i = 0; i < servers.size(); i++)
        _binders.insert(std::pair<int, std::string>(servers[i].getPort(),servers[i].getIp()));
	// std::map<int, const char*>::iterator it = _binders.begin();
	// std::map<int, const char*>::iterator end = _binders.end();
	//std::cout << " hello " << _binders[8001] << std::endl;
	// for(; it != end; ++it)
	// 	std::cout << "Binding to " << it->second << " on port " << it->first << std::endl;
}

// ! /////////////////////// Errors Managers //////////////////////

void Config::main_error_check()
{
    check_brekets();
}

void Config::check_brekets()
{
    int open;
    open = 0;
    for (int i = 0; full_file[i]; i++)
    {
        if (full_file[i] == '{')
            open++;
        if (full_file[i] == '}')
            open--;
    }
    if (open != 0)
        throw NotclosedBrackets();
}


void Config::init_statusCodeMessages()//TODO put it in a better place
	{
		statusCodeMessages["100"] = "Continue";
		statusCodeMessages["200"] = "OK";
		statusCodeMessages["201"] = "Created";
		statusCodeMessages["202"] = "Accepted";
		statusCodeMessages["203"] = "Non-Authoritative Information";
		statusCodeMessages["204"] = "No Content";
		statusCodeMessages["205"] = "Reset Content";
		statusCodeMessages["206"] = "Partial Content";
		statusCodeMessages["300"] = "Multiple Choices";
		statusCodeMessages["301"] = "Moved Permanently";
		statusCodeMessages["302"] = "Found";
		statusCodeMessages["303"] = "See Other";
		statusCodeMessages["304"] = "Not Modified";
		statusCodeMessages["305"] = "Use Proxy";
		statusCodeMessages["307"] = "Temporary Redirect";
		statusCodeMessages["400"] = "Bad Request";
		statusCodeMessages["401"] = "Unauthorized";
		statusCodeMessages["403"] = "Forbidden";
		statusCodeMessages["404"] = "Not Found";
		statusCodeMessages["405"] = "Method Not Allowed";
		statusCodeMessages["406"] = "Not Acceptable";
		statusCodeMessages["407"] = "Proxy Authentication Required";
		statusCodeMessages["408"] = "Request Time-out";
		statusCodeMessages["409"] = "Conflict";
		statusCodeMessages["410"] = "Gone";
		statusCodeMessages["411"] = "Length Required";
		statusCodeMessages["412"] = "Precondition Failed";
		statusCodeMessages["413"] = "Request Entity Too Large";
		statusCodeMessages["414"] = "Request-URI Too Large";
		statusCodeMessages["415"] = "Unsupported Media Type";
		statusCodeMessages["416"] = "Requested range not satisfiable";
		statusCodeMessages["417"] = "Expectation Failed";
		statusCodeMessages["500"] = "Internal Server Error";
		statusCodeMessages["501"] = "Not Implemented";
		statusCodeMessages["502"] = "Bad Gateway";
		statusCodeMessages["503"] = "Service Unavailable";
		statusCodeMessages["504"] = "Gateway Time-out";
		statusCodeMessages["505"] = "HTTP Version not supported";
	}
	void Config::init_mimetype()
	{
		mimetypeMap["txt"]= "text/plain";
        mimetypeMap["pdf"]= "application/pdf";
        mimetypeMap["html"]= "text/html";
        mimetypeMap["htm"]= "text/html";
        mimetypeMap["xml"]= "text/xml";
        mimetypeMap["js"]= "application/x-javascript";
        mimetypeMap["xhtml"]= "application/xhtml+xml";
        mimetypeMap["svg"]= "image/svg+xml";
        mimetypeMap["svgz"]= "image/svg+xml";
        mimetypeMap["jpg"]= "image/jpeg";
        mimetypeMap["jpeg"]= "image/jpeg";
        mimetypeMap["png"]= "image/png";
        mimetypeMap["tif"]= "image/tiff";
        mimetypeMap["tiff"]= "image/tiff";
        mimetypeMap["ico"]= "image/ico";
        mimetypeMap["cur"]= "image/ico";
        mimetypeMap["bmp"]= "image/bmp";
        mimetypeMap["wml"]= "text/vnd.wap.wml";
        mimetypeMap["wmlc"]= "application/vnd.wap.wmlc";
	}
