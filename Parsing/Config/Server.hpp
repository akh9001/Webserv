/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 14:33:33 by mokhames          #+#    #+#             */
/*   Updated: 2022/06/25 00:17:51 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webServer.hpp"
//#include "Location.hpp"

class Location;

class TlisternerServer
{
    private : 
        std::string ip; // may be char *
        int port;
    public :
        TlisternerServer()
        {
            ip = "127.0.0.1";
            port = 80;
        }
    
        TlisternerServer(const std::string ip, const int port) : ip(ip), port(port) {}
        TlisternerServer &operator=(const TlisternerServer &t)
        {
            //std::cout << "TlisternerServer" << std::endl;
            ip = t.ip;
            port = t.port;
            return *this;
        }
        std::string getIp() const { return ip; }
        int getPort() const { return port; }
        void setPort(int port) { this->port = port;}
        void setIp(const std::string& ip) {this->ip = ip;}
        
};

class Server
{
    private:
        std::vector<std::string> serverline;
        std::vector<std::string> serverName;
        TlisternerServer _listen;
        std::vector<Location> _locations; // can be made as map
       // std::map<std::string, Location> _locations1;
    protected :

        std::string root;
        std::vector<std::string>                _allowed_methods;
        std::vector<std::string>                 _index;
        std::map<int , std::string>              redirect_uri;
        std::map<int , std::string> errorPages;
        std::string cgiPath;
        std::string uploadPath;
        unsigned long _clinet_max_body_size;
        bool _autoindex;
        bool default_serv;

    public :
    // ! //////////////////////////  Exception classes 
        class NotacceptableError : public std::exception
        {
            const char * what() const throw()
            {
                return "Not acceptable syntax error";
            }
        };
        class WrongMethod : public std::exception
        {
            const char * what() const throw()
            {
                return "wrong HTTP Method";
            }
        };


         // ! Constuctors and destructor
        Server();
        ~Server();
        Server(Server const& c);
        Server &operator=(Server const& c);

        // ! /////////////////////// Getters & Setters //////////////////////
        std::string getRoot() const { return this->root; }
        void setRoot(const std::string& root) { this->root = root; }
        std::vector<std::string> getServerLine() const { return serverline;}
        void setServerline(std::vector<std::string> c);
        std::vector<std::string> getServerName() const { return serverName;}
        void setSeverName(std::vector<std::string> &c) { serverName = c;}
        int getPort() const { return _listen.getPort();}
        void setPort(int port) { _listen.setPort(port);}
        std::string getIp() const { return _listen.getIp();}
        void setIp(std::string ip) { _listen.setIp(ip);}
        std::map<int , std::string> getRedirectUri() const { return redirect_uri;}
        //void setredirectUri(std::string uri) { redirect_uri = uri;}
        std::string getCgiPath() const { return cgiPath;}
        void setCgiPath(std::string cgiPath) { this->cgiPath = cgiPath;}
        std::vector<Location> getLocation() const { return _locations;}
        void setLocation(std::vector<Location> c) { _locations = c;}
        std::vector<std::string> getAllowedMethods() const { return _allowed_methods;}
        void setAllowedMethods(std::vector<std::string> methods) { _allowed_methods = methods;}
        std::vector<std::string> getIndex() const { return _index;}
        void setIndex(std::vector<std::string> index) { _index = index;}
        unsigned long getClientMaxBodySize() const { return _clinet_max_body_size;}
        void setClientMaxBodySize(unsigned long c) { _clinet_max_body_size = c;}
        bool getAutoIndex() const { return _autoindex;}
        void setAutoIndex(bool f) { _autoindex = f;}
        std::map<int, std::string> &getErrorPages()  { return errorPages;}
        void setErrorPages(std::map<int, std::string> &c) { errorPages = c;}
        std::string getUploadPath() const { return uploadPath;}
        void setUploadPath(const std::string &uploadPath) { this->uploadPath = uploadPath;}
        

        




        // ! /////////////////////// Parse Lines //////////////////////
        void parseLines();
        void fetch_server_name(std::string&);
        void fetch_host(std::string& c);
        void fetch_root(std::string& c);
        void fetch_index(std::string& c);
        int fetch_location(std::vector<std::string>::iterator it);
        void fetch_cbbs(std::string& c);
        void fetch_allowed_methods(std::string& c);
        void fetch_autoindex(std::string& c);
        void fetch_redirect(std::string& c);
        void fetch_errorPage(std::string& c);
        void fetch_cgi(std::string& c);
        void fetchErrorPage(std::string& c);
        void fetch_upload(std::string& c);
        int spaceCount(std::string &c, int a) const;

        // ! ///////////////////////////// Error  ///////////////////
        int parsePath(std::string& path);
        // ! ///////////////////////////// checkers  /////////////////////
        bool checkServerName(std::string& c);
       // void checkStatusCode(std::string& c);
};

// class Location : public Server
// {
//         private :
//                 std::string location_match;
//                 std::string prefix_match;
//                 //std::string root1;
//                 std::vector<Location> _locations;
//         public :
        
//         // ! Constuctors and destructor
//         Location() {}
//         ~Location() {}
//         Location(Location const& c) {*this = c;}
//         Location &operator=(Location const& c) { 
//             //std::cout << "hnaa " << std::endl; 
//             location_match = c.location_match;
//              prefix_match = c.prefix_match;
//              this->root = c.root;
//              this->_allowed_methods = c._allowed_methods;
//              this->_index = c._index;
//              this->_clinet_max_body_size = c._clinet_max_body_size;
//              this->_autoindex = c._autoindex;
//             return *this; 
//             }

//         // ! /////////////////////// Getters & Setters //////////////////////
//         std::string getLocation_match() const { return location_match;}
//         void setLocation_match(std::string c) { location_match = c;}
//         // std::string getRoot() const { return root;}
//         // void setRoot(const std::string &c) {root = c;}

// };

#endif