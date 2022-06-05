#pragma once
#include "webserver.hpp"


class Request
{
    private :
        std::string requestFile;
        std::string full_file;
        std::vector<std::string> headerPart;
        std::vector<std::string> bodyPart;

        std::string method;
        std::string uri;
        std::string version;
        std::string host;
        std::string connection;
        std::string user_agent;
        std::string accept;
        std::string accept_encoding;
        std::string accept_language;
        std::string accept_charset;
        std::string content_type;
        std::string content_length;


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
    Request(std::string file);
    ~Request();
    Request(Request const& c);
    Request &operator=(Request const& c);

      // ! /////////////////////// Getters & Setters //////////////////////

    // ! /////////////////////// file_Readers //////////////////////

        void main_read();
        void readFile();
        void split_parts();
		int parse_read(std::string c);
    // ! /////////////////////// parse header //////////////////
   void  parse_header();
   void parsefline();


   // ! /////////////////////// erros check //////////////////
    void main_error_check();
    int checkMethod();
    int checkURI();
    int checkVersion();
};
