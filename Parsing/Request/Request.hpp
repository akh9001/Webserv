/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 14:33:38 by mokhames          #+#    #+#             */
/*   Updated: 2022/06/24 23:52:20 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "../webServer.hpp"
#include "../../FileHandler/fileHandler.hpp"
#include "../Config/Config.hpp"  
#include "../Config/Server.hpp"  
#include "../Config/Location.hpp"

class CGI;
class Config;
// class Server;
// class Location;

class Request
{
         private :

                std::string requestFile;
                std::string header;
                std::string full_file;
                std::vector<std::string> headerPart;
                std::vector<std::string> bodyPart;
        
                std::string method;
                std::string uri;      // uri = path + query_string
                std::string version;
                std::string query;
                std::string hostIp;
                int hostPort;
        
                std::string content_type;
                Server server;
                Location location;
                
                long long contentLength;
                std::map<std::string, std::string> headerMap;
                std::map<std::string, std::string> CoockieMap;
        
        
                int fchuncked;
                long long read;
                std::string save;
                std::string tmpUri;
                int full;
                bool parsed;
                std::fstream file;
                std::string filePath;
                int change;
	public:
				CGI		*cgi_ptr;
        
         public :


                class fileErrorException : public std::exception
                {
                    const char * what() const throw()
                    {
                        return " : File Not Found";
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
                Request();
                Request(std::string file);
                Request(std::string file, int a);
                ~Request();
                Request(Request const& c);
                Request &operator=(Request const& c);
        
                // ! /////////////////////// Getters & Setters //////////////////////
                std::map<std::string, std::string> getHeaderMap() const { return headerMap; }
                long long getContentLenth() const { return contentLength; }
                std::string getMethod() const { return method; }
                std::string getUri() const { return uri; }
                std::string getVersion() const { return version; }
                std::string getHostIp() const { return hostIp; }
                int getHostPort() const { return hostPort; }
                std::string getQuery() const { return query; }
        
                Server getServer() const { return server; }
                Location getLocation() const { return location; }
                std::string const &getFilePath() const { return filePath; }
                 // ! /////////////////////// file_Readers //////////////////////
            
                void main_read();
                void readFile();
                void split_parts();
                bool parseChunks(std::string c, Config config);
                 // ! /////////////////////// parse header //////////////////
                int parse_header(std::string c);
                int parse_body(std::string c);
            
                void  print_header();
                void parsefline(std::string &);
                void parseHeaderLines(Config config);
                void parseUri();
                void parseCookies();
                // ! ////////////////////// fetch server or location //////////////////////
                void fetchContentLength();
                void fetchHost();
                void getRightServer(Config config);
                int getRightLocation();
                 // ! /////////////////////// erros check //////////////////
                  void main_error_check();
                  int checkMethod();
                  int checkURI();
                  int checkVersion();
                  void checkContentLength(int a);
                  void checkTransferEncoding();
                 // ! ////////////////////// clear //////////////////
                  void clear();
};


#include "../../Includes/cgi.hpp"
#endif