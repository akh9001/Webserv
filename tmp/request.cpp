#include "request.hpp"

 // ! Constuctors and destructor
Request::Request()
{
	this->requestFile = "";
	this->full_file = "";
	this->headerPart = std::vector<std::string>();
	this->bodyPart = std::vector<std::string>();
	this->method = "";
	this->uri = "";
	this->version = "";
	this->host = "";
	this->connection = "";
	this->user_agent = "";
	this->accept = "";
	this->accept_encoding = "";
	this->accept_language = "";
	this->accept_charset = "";
	this->content_type = "";
	this->content_length = "";
}
Request::Request(std::string file)
{
    requestFile = file;
}
Request::~Request()
{
}
Request::Request(Request const& c)
{
    *this = c;
}
Request &Request::operator=(Request const& c)
{
    requestFile = c.requestFile;
    requestFile = c.requestFile;
    headerPart = c.headerPart;
    bodyPart = c.bodyPart;
    return *this;
}


// ! /////////////////////// file_Readers //////////////////////

void Request::main_read()
{
    readFile();
    split_parts();
    parsefline();
    main_error_check();
}

void Request::readFile()
{
    std::fstream a(requestFile);

    if (!a.is_open())
    {
        std::cout << "Error : " << requestFile;
        throw fileErrorException();
    }
    std::string output;
    while (std::getline(a, output, '\n'))
        full_file += output;

}


void    Request::split_parts()
{
    std::string line;
    std::string full_file1(full_file);
    size_t pos = 0;
    while ((pos = full_file1.find("\r\n")) != std::string::npos)
    {
        line = full_file1.substr(0, pos);
        if (line[0] == '\0')
            break;
        headerPart.push_back(line);
        full_file1.erase(0, pos + 1);
    }
    while ((pos = full_file1.find("\n")) != std::string::npos)
    {
        line = full_file1.substr(0, pos);
        if (line[0] == '\0')
            break;
        bodyPart.push_back(line);
        full_file1.erase(0, pos + 1);
    }
    bodyPart.push_back(line);
    full_file1.erase(0, pos + 1);
}

int Request::parse_read(std::string c)
{
	(void)c;
	return 1;	
}
 // ! /////////////////////// parse header //////////////////

 void Request::parse_header()
 {
     parsefline();
 }
 void Request::parsefline()
 {
    std::string line = headerPart[0];
    std::string tmp;
    size_t pos = 0;
    // get method from headerPart[0]
    pos = line.find(" ");
    method = line.substr(0, pos);
    line.erase(0, pos + 1);
    // get uri from headerPart[0]
    pos = line.find(" ");
    uri = line.substr(0, pos);
    line.erase(0, pos + 1);
    // get version from headerPart[0]
    pos = line.find(" ");
    version = line.substr(0, pos);
    line.erase(0, pos + 1);
}
// ! /////////////////////// erros check //////////////////
    
    void Request::main_error_check()
    {
        checkMethod();
        checkURI();
        checkVersion();
    }

    int Request::checkMethod()
    {
        if (method != "GET" || method != "POST" || method != "DELETE")
        {
            throw WrongMethod();
            return (405);
        }
		return 200;
    }

    int Request::checkURI()
    {
        std::string allowedchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
        for (size_t i = 0; i < uri.size(); i++)
        {
            if (allowedchars.find(uri[i]) == std::string::npos)
            {
                return (400);
            }
        }
		return 200;
        
    }

    int Request::checkVersion()
    {
        if (version != "HTTP/1.1" || version != "HTTP/1.0")
        {
            return (400); // 505 HTTP Version Not Supported
        }
		return 200;
    }