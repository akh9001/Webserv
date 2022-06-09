#include "../webServer.hpp"
#include "../../FileHandler/fileHandler.hpp"

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
        std::string host;
        std::string connection;
        std::string user_agent;
        std::string accept;
        std::string accept_encoding;
        std::string accept_language;
        std::string accept_charset;
        std::string content_type;
        
        int content_length;
        std::map<std::string, std::string> headerMap;


        static int fchuncked;
        static std::string save;
        static int full;
        bool parsed;
       // ws::fileHandler fh;
        std::fstream file;
        std::string filePath;
        static int change;
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
    // ! /////////////////////// file_Readers //////////////////////

        void main_read();
        void readFile();
        void split_parts();
        int parseChunks(std::string c);
    // ! /////////////////////// parse header //////////////////
    int parse_header(std::string c);
    int parse_body(std::string c);

   void  print_header();
   void parsefline(std::string &);
   void parseHeaderLines();


   // ! /////////////////////// erros check //////////////////
    void main_error_check();
    int checkMethod();
    int checkURI();
    int checkVersion();
// ! ////////////////////// clear //////////////////
    void clear();
};