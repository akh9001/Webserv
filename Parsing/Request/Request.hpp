#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "../webServer.hpp"
#include "../../FileHandler/fileHandler.hpp"
#include "../Config/Config.hpp"  
#include "../Config/Server.hpp"  
#include "../Config/Location.hpp"

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
        std::string uri;
        std::string version;
        std::string hostIp;
        int hostPort;
        std::string connection;
        std::string user_agent;
        std::string accept;
        std::string accept_encoding;
        std::string accept_language;
        std::string accept_charset;
        std::string content_type;
        Server server;
        Location location;
        
        long long contentLength;
        std::map<std::string, std::string> headerMap;


         int fchuncked;
        long long read;
         std::string save;
        std::string tmpUri;
         int full;
        bool parsed;
       // ws::fileHandler fh;
        std::fstream file;
        std::string filePath;
         int change;
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
     int getContentLenth() const { return contentLength; }
    Server getServer() const { return server; }
    Location getLocation() const { return location; }

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
    // ! ////////////////////// fetch server or location //////////////////////
    void fetchContentLength();
    void fetchHost();
    void getRightServer(Config config);
    void getRightLocation();


   // ! /////////////////////// erros check //////////////////
    void main_error_check();
    int checkMethod();
    int checkURI();
    int checkVersion();
    void checkContentLength(int a);
// ! ////////////////////// clear //////////////////
    void clear();
};
#endif