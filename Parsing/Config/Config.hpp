#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../webServer.hpp"
// #include "Server.hpp"
class Server;

class Config
{
    protected:
        std::string file_name;

    private:
        std::string full_file;
        std::vector<std::string> lines;
        std::vector<Server> servers;
<<<<<<< HEAD
        std::map<int, const char *> _binders;
=======
        std::map<int, std::string> _binders;
>>>>>>> akhalidy

    public :

    class fileErrorException : public std::exception
    {
        const char * what() const throw()
        {
            return " : File Not Found";
        }
    };
    class NotclosedBrackets : public std::exception
    {
        const char * what() const throw()
        {
            return "Brackets are not closed";
        }
    };
    class NoServerFoundException : public std::exception
    {
        const char * what() const throw()
        {
            return "No Server found";
        }
    };

    class MultpiteServerName : public std::exception
    {
        const char * what() const throw()
        {
            return "Multiple servers with same server name";
        }
    };
    // ! Constuctors and destructor
    Config(std::string file);
    ~Config();
    Config(Config const& c);
    Config &operator=(Config const& c);
    // ! /////////////////////// Getters & Setters //////////////////////
    std::vector<Server> getServers() const;
    
    std::string getFullFile() const;
    std::vector<std::string> getLines() const;
    std::string getLine(std::string &) const;
    //Server getServer(int port) const;
    Server getServer(int port, std::string &) const;
    void setfile(std::string&);
<<<<<<< HEAD
    std::map<int, std::string> getBinders() const;
=======
    std::map<int, std::string > getBinders() const;
>>>>>>> akhalidy
    // ! /////////////////////// file_Readers //////////////////////
    void main_read();
    void read_file();
    void split_line();
    void split_servers();
    void parse_server();
    void parse_bind_map();

    // ! /////////////////////// Errors Managers //////////////////////
    void main_error_check();
    void check_brekets();
};

#endif