#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <map>

// ! //////////////////// Constuctors and destructor /////////////////

Config::Config(std::string file) : file_name(file) , full_file(), lines(), servers()
{
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
    for (int i = 0; i < servers.size(); i++)
    {
        if (servers[i].getPort() == port)
            a.push_back(servers[i]);
    }
    // std::cout << "Server " << std::endl;
    if (!c.size() && a.size())
        return a[0];
    else if (!c.size())
            throw NoServerFoundException();
    for (int i = 0; i < a.size(); i++)
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
    for (int i = 0; i < servers.size(); i++)
        servers[i].parseLines();
}

void Config::parse_bind_map()
{
    for (int i = 0; i < servers.size(); i++)
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
