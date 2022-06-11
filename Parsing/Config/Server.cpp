#include "Server.hpp"
#include "Location.hpp"

  // ! Constuctors and destructor
 Server::Server() : serverline(), serverName(), _listen(), _locations(), root(), _allowed_methods(), _index(), redirect_uri(), errorPages(), cgiPath(), _autoindex() {

 }
 Server::~Server() {}
 Server::Server(Server const& c)
 {
     *this = c;
 }
 Server &Server::operator=(Server const& c)
 {
<<<<<<< HEAD
=======
    //std::cout << "Server::operator=(Server)" << std::endl;
    //std::cout << c.getIp() << std::endl;
>>>>>>> akhalidy
    serverline.assign(c.serverline.begin(), c.serverline.end());
    serverName.assign(c.serverName.begin(), c.serverName.end());
    _listen = c._listen;
    _locations.assign(c._locations.begin(), c._locations.end());
    root = c.root;
    _allowed_methods = c._allowed_methods;
    _index = c._index;
    redirect_uri = c.redirect_uri;
    errorPages = c.errorPages;
    cgiPath = c.cgiPath;
    _clinet_max_body_size = c._clinet_max_body_size;
    _autoindex = c._autoindex;
<<<<<<< HEAD
=======
   // std::cout << _listen.getIp() << std::endl;
>>>>>>> akhalidy
    return *this;

 }

  // ! /////////////////////// Getters & Setters //////////////////////
 void Server::setServerline(std::vector<std::string> c)
 {
     serverline.assign(c.begin(), c.end());
 }
 // ! /////////////////////// Parse Lines //////////////////////

void Server::parseLines()
{
    std::vector<std::string>::iterator it = serverline.begin();
<<<<<<< HEAD
    it +=2;
    for (;it != serverline.end();it++)
    {
=======
    
    it +=2;
    for (;it != serverline.end();it++)
    {
       // std::cout << *it << std::endl;
>>>>>>> akhalidy
        if ((*it).find("server_name") != std::string::npos)
            fetch_server_name(*it);
        else if ((*it).find("listen") != std::string::npos)
            fetch_host(*it);
        else if ((*it).find("root") != std::string::npos)
            fetch_root(*it);
        else if ((*it).find("allow_methods") != std::string::npos)
             fetch_allowed_methods(*it);
        else if ((*it).find("client_body_buffer_size") != std::string::npos)
            fetch_cbbs(*it);
        else if ((*it).find("redirect") != std::string::npos)
            fetch_redirect(*it);
        else if ((*it).find("cgi_path") != std::string::npos)
            fetch_redirect(*it);
        else if ((*it).find("autoindex") != std::string::npos)
            fetch_autoindex(*it);
        else if ((*it).find("index") != std::string::npos)
            fetch_index(*it);
        else if ((*it).find("location") != std::string::npos)
            it += fetch_location(it);
        else if ((*it).find("error_page ") != std::string::npos)
            fetchErrorPage(*it);
        else if ((*it).find("}") != std::string::npos)
                continue;
        else
            throw NotacceptableError();
    }
<<<<<<< HEAD

  //  std::cout << "--------------------------------" << _locations[0].getRoot() <<std::endl;
=======
>>>>>>> akhalidy
}

 void Server::fetch_server_name(std::string& c)
 {
    std::string tmp = c.substr(12, c.size() - 12);
    size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        serverName.push_back(tmp.substr(0,pos));
        tmp.erase(0, pos + 1);
    }
    serverName.push_back(tmp.substr(0,pos));
    tmp.erase(0, pos + 1);
 }
 void Server::fetch_host(std::string& c)
 {
    std::string tmp = c.substr(7, c.size() - 7);
    size_t pos = 0;
     while ((pos = tmp.find(":")) != std::string::npos)
    {
        setIp(tmp.substr(0,pos));
        tmp.erase(0, pos + 1);
    }
<<<<<<< HEAD
    setPort(stoi(tmp.substr(0,pos)));
=======
    int i = 0;
    std::istringstream(tmp.substr(0,pos)) >> i;
    sscanf(tmp.substr(0,pos).c_str(), "%d", &i);
    setPort(i);
>>>>>>> akhalidy
    tmp.erase(0, pos + 1);
}

 void Server::fetch_redirect(std::string& c)
 {
     //std::cout << "root insde fetch " <<  c << std::endl;
    this->setredirectUri(c.substr(9, c.size() - 9));

 }
 void Server::fetch_cgi(std::string& c)
 {
     //std::cout << "root insde fetch " <<  c << std::endl;
    this->setCgiPath(c.substr(9, c.size() - 9));

 }

 void Server::fetch_root(std::string& c)
 {
     //std::cout << "root insde fetch " <<  c << std::endl;
    this->setRoot(c.substr(5, c.size() - 5));

 }
 void Server::fetch_index(std::string& c)
 {
    setIndex(c.substr(6, c.size() - 6));
 }
int Server::fetch_location(std::vector<std::string>::iterator it)
{
    int i = 1;
    Location tmp(*this);
    size_t pos = 0;
    // _locations.resize(_locations.size() + 1);
    std::string a = (*it).substr(9, it->size() - 9);
    pos = a.find(" ");
    tmp.setLocation_match(a.substr(0, pos));
    it++;
    while (*it != "}")
    {
        if ((*it).find("root") != std::string::npos)
            tmp.fetch_root(*it);
        else if ((*it).find("allow_methods") != std::string::npos)
            tmp.fetch_allowed_methods(*it);
        else if ((*it).find("client_body_buffer_size") != std::string::npos)
            tmp.fetch_cbbs(*it);
        else if ((*it).find("autoindex") != std::string::npos)
            tmp.fetch_autoindex(*it);
        else if ((*it).find("index") != std::string::npos)
            tmp.fetch_index(*it);
        else if ((*it).find("error_page ") != std::string::npos)
            fetchErrorPage(*it);
        else if ((*it).find("redirect") != std::string::npos)
            fetch_redirect(*it);
        else if ((*it).find("cgi_path") != std::string::npos)
            fetch_redirect(*it);
        else
             throw NotacceptableError();
        i++;
        it++;
    }
    _locations.push_back(tmp);
    return i;
}
 void Server::fetch_allowed_methods(std::string& c)
 {
    std::string tmp = c.substr(14, c.size() - 14);
    size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        std::string str = tmp.substr(0,pos);
        if (str != "GET" && str != "POST" && str != "PUT" && str != "DELETE")
            throw WrongMethod();
        _allowed_methods.push_back(str);
        tmp.erase(0, pos + 1);
    }
    _allowed_methods.push_back(tmp.substr(0,pos));
    tmp.erase(0, pos + 1);
 }
 void Server::fetch_cbbs(std::string& c)
 {
    std::string tmp = c.substr(24, c.size() - 24);
<<<<<<< HEAD
    unsigned long a = stoi(tmp);
    setClientMaxBodySize(a);

=======
    unsigned long i = 0;
    std::istringstream(tmp) >> i;
    sscanf(tmp.c_str(), "%ld", &i);
    setClientMaxBodySize(i);
>>>>>>> akhalidy
 }

 void Server::fetch_autoindex(std::string& c)
 {
    std::string tmp = c.substr(10, c.size() - 10);
    if (tmp == "on")
        setAutoIndex(true);
    else
        setAutoIndex(false);
}

void Server::fetchErrorPage(std::string& c)
{
    std::string tmp = c.substr(12, c.size() - 12);
    int i = 0;
    for (;tmp[i] == ' '; i++);
    tmp = tmp.substr(i, tmp.size() - i);
    std::vector<std::string> a;
     size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        a.push_back(tmp.substr(0, pos));
        tmp.erase(0, pos + 1);
    }
    a.push_back(tmp.substr(0, pos));
    tmp.erase(0, pos + 1);
    if (a.size() != 2)
        throw NotacceptableError();
    if (a[0].size() != 3)
        throw NotacceptableError();
<<<<<<< HEAD
    int b = stoi(a[0]);
    errorPages.insert(std::make_pair(b, a[1]));
=======
    int q = 0;
    std::istringstream(a[0]) >> q;
    sscanf(a[0].c_str(), "%d", &q);
    errorPages.insert(std::make_pair(q, a[1]));
>>>>>>> akhalidy
}
// ! ///////////////////////////// Error pages ////////////////////

// void Server::checkStatusCode(std::string& c)
// {
    
// }

  // ! ///////////////////////////// checkers  /////////////////////
    bool Server::checkServerName(std::string& c)
    {
        for (int i = 0; i < serverName.size(); i++)
        {
            if (serverName[i] == c)
                return true;
        }
        return false;
    }

