#include "Request.hpp"
// #include "fileHandler.hpp"
#include "../../Includes/networking.hpp"
#include "../Config/Config.hpp"  
#include "../Config/Server.hpp"  
#include "../Config/Location.hpp"

int Request::fchuncked = 0;
std::string Request::save = "";
int Request::change = 0;
int Request::full = 0;

 // ! Constuctors and destructor
Request::Request()
{
    this->requestFile = "";
    this->header = "";
    this->full_file = "";
    this->headerPart.clear();
    this->bodyPart.clear();
    this->method = "";
    this->uri = "";
    this->version = "";
    hostIp = "127.0.0.1";
    hostPort = 8080;
    this->connection = "";
    this->user_agent = "";
    this->accept = "";
    this->accept_encoding = "";
    this->accept_language = "";
    this->accept_charset = "";
    this->content_type = "";
    this->contentLength = 0;
    this->read = 0;
    this->parsed = false;
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
    if (this != &c)
    {
        requestFile = c.requestFile;
        header = c.header;
        full_file = c.full_file;
        headerPart = c.headerPart;
        bodyPart = c.bodyPart;
        method = c.method;
        uri = c.uri;
        version = c.version;
        hostIp = c.hostIp;
        hostPort = c.hostPort;
        connection = c.connection;
        user_agent = c.user_agent;
        accept = c.accept;
        accept_encoding = c.accept_encoding;
        accept_language = c.accept_language;
        accept_charset = c.accept_charset;
        content_type = c.content_type;
        contentLength = c.contentLength;
        parsed = c.parsed;
    }
    return *this;
}


// ! /////////////////////// file_Readers //////////////////////

// void Request::main_read()
// {
//     readFile();
//     split_parts();
//     parsefline();
//     main_error_check();
// }

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
}

bool Request::parseChunks(std::string c, Config config)
{
    if (change == 0)
        parse_header(c);
    if (change == 1 && parsed == false)
    {
        parseHeaderLines(config);
        // contentLength = 9999999;
        // read = contentLength;
    }
    else if (change == 1)
        parse_body(c);
    checkContentLength(c.size());
    std::cout << "i am here "  << read << std::endl;
    if (read <= 0)
        return true;
    return false;
}

int Request::parse_header(std::string c)
{
    size_t pos = 0;
    std::string line(c);
    std::string tmp;
    int i = 0;
    if ((pos = line.find("\r\n") == std::string::npos))
       save += line;
    else
    {
        while ((pos = line.find("\r\n")) != std::string::npos)
        {
            i++;
            if (fchuncked == 0)
            {
                fchuncked = 1;
                tmp = line.substr(0, pos);
                save += tmp;
                parsefline(save);
                headerPart.push_back(save);
                save = "";
            }
            else
            {
                tmp = line.substr(0, pos);
                if (tmp == "\0")
                {
                    line.erase(0, pos + 2);
                    save = line;
                    change = 1;
                    filePath = ws::fileHandler::createTmp("request_tmp_files/");
                    break;
                }
                save += line;
                headerPart.push_back(save);
                save = "";
            }
            line.erase(0, pos + 2);
        }
        save = line;
    }
    return 0;
}

void Request::parseHeaderLines(Config config)
{

    size_t pos = 0;
    // std::cout << "i am here" << std::endl;
    for (int i = 1; i < headerPart.size(); i++)
    {
        if ((pos = headerPart[i].find(":")) != std::string::npos)
            headerMap[headerPart[i].substr(0, pos)] = headerPart[i].substr(pos + 2, headerPart[i].find("\r\n") - pos - 2);   
    }
    fetchContentLength();

    getRightServer(config);
    getRightLocation();
    checkContentLength(0);
    parsed = true;
}

int Request::parse_body(std::string c)
{
    if (save.size() > 0)
    {
        save += c;
        ws::fileHandler::write(filePath, save);
    }
    else
        ws::fileHandler::write(filePath, c);
    save = "";
    return 0;
}

 // ! /////////////////////// parse header //////////////////

 void Request::print_header()
 {
     for (int i = 0; i < headerPart.size(); i++)
        std::cout <<  "|" << headerPart[i] <<  "|" << std::endl;
    std::cout << save << std::endl;
 }
 
 void Request::parsefline(std::string &c)
 {
        std::string line (c);
        size_t pos = 0;
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
        main_error_check();
        tmpUri = uri;

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
        if (method != "GET" && method != "POST" && method != "DELETE")
        {
            throw WrongMethod();
            return (405);
        }
        return (200);
    }

    int Request::checkURI()
    {
        std::string allowedchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
        for (int i = 0; i < uri.size(); i++)
        {
            if (allowedchars.find(uri[i]) == std::string::npos)
                return (400);
        }

        return (200);
    }

    int Request::checkVersion()
    {
        if (version != "HTTP/1.1" || version != "HTTP/1.0")
        {
            return (400); // 505 HTTP Version Not Supported
        }
        return (200);
    }

    void Request::checkContentLength(int a)
    {
        if (contentLength > location.getClientMaxBodySize()
            || contentLength < 0)
        {
           std::cout << "Content-Length: " << contentLength << std::endl;
           std::cout << "a "  << location.getClientMaxBodySize() << std::endl;
            throw "413";
        }
        read -= a;
    }

    void Request::clear()
    {
        headerPart.clear();
        headerMap.clear();
        save = "";
        filePath = "";
        parsed = false;
        method = "";
        hostIp = "127.0.0.1";
        hostPort = 8080;
        uri = "";
        version = "";
        fchuncked = 0;
        change = 0;
    }


     // ! ////////////////////// fetch server or location //////////////////////

    void Request::fetchContentLength()
    {
        long long i = 0;
        std::istringstream(headerMap["Content-Length"]) >> i;
        sscanf(headerMap["Content-Length"].c_str(), "%lld", &i);
        contentLength = i;
        read = contentLength;
    }

     void Request::fetchHost()
     {
        hostIp = headerMap["Host"].substr(0, headerMap["Host"].find(":"));
        hostPort = atoi(headerMap["Host"].substr(headerMap["Host"].find(":") + 1, headerMap["Host"].size()).c_str());
     }

    void  Request::getRightServer(Config config)
    {
        try {
        server = config.getServer(hostPort, hostIp);
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    void  Request::getRightLocation()
    {
        size_t pos = 0;
        std::string clone(tmpUri);
        int n = server.getLocation().size();
        for (int i = 0; i < n; i++) {
            if (server.getLocation()[i].getLocation_match() == tmpUri) {
                location = server.getLocation()[i];
                return ;
            }
        }
        while (clone.size() > 0 && (pos = clone.find("/")) != std::string::npos && pos != 0)
            clone.erase(0, pos);
        tmpUri.erase(0, pos);
        if (pos != 0)
            getRightLocation();
        else
            throw "404";
    }

