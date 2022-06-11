#include "Request.hpp"
// #include "fileHandler.hpp"
#include "../../Includes/networking.hpp"
<<<<<<< HEAD
=======
#include "../Config/Config.hpp"  
#include "../Config/Server.hpp"  
#include "../Config/Location.hpp"
>>>>>>> akhalidy

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
<<<<<<< HEAD
    this->host = "";
=======
    hostIp = "127.0.0.1";
    hostPort = 8080;
>>>>>>> akhalidy
    this->connection = "";
    this->user_agent = "";
    this->accept = "";
    this->accept_encoding = "";
    this->accept_language = "";
    this->accept_charset = "";
    this->content_type = "";
<<<<<<< HEAD
    this->content_length = 0;
=======
    this->contentLength = 0;
    this->read = 0;
>>>>>>> akhalidy
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
<<<<<<< HEAD
        host = c.host;
=======
        hostIp = c.hostIp;
        hostPort = c.hostPort;
>>>>>>> akhalidy
        connection = c.connection;
        user_agent = c.user_agent;
        accept = c.accept;
        accept_encoding = c.accept_encoding;
        accept_language = c.accept_language;
        accept_charset = c.accept_charset;
        content_type = c.content_type;
<<<<<<< HEAD
        content_length = c.content_length;
=======
        contentLength = c.contentLength;
>>>>>>> akhalidy
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
<<<<<<< HEAD
    // while ((pos = full_file1.find("\n")) != std::string::npos)
    // {
    //     line = full_file1.substr(0, pos);
    //     if (line[0] == '\0')
    //         break;
    //     bodyPart.push_back(line);
    //     full_file1.erase(0, pos + 1);
    // }
    // bodyPart.push_back(line);
    // full_file1.erase(0, pos + 1);
}


// int Request::parseChunks(std::string c)
// {
//     size_t pos = 0;
//     std::string line(c);
//     if ((pos = line.find("\r\n")) != std::string::npos && fchuncked == 0)
//     {
//         fchuncked = 1;
//         headerPart[headerPart.size() - 1] += line;
//         parsefline(headerPart[headerPart.size() - 1]);
//     }
//     else if ((pos = line.find("\r\n")) != std::string::npos && fchuncked == 1)
//     {
//         if ((pos = line.find("\r\n\r\n")) == std::string::npos)
//             headerPart[headerPart.size() - 1] += line;
//             // headerPart.push_back(line.substr(0, pos));
//     }
//     else
//     {
//         if (headerPart.size() <= 0)
//             headerPart.push_back(line);
//         else
//             headerPart[headerPart.size() - 1] += line;
//     }
//     return 0;

// }


int Request::parseChunks(std::string c)
=======
}

bool Request::parseChunks(std::string c, Config config)
>>>>>>> akhalidy
{
    if (change == 0)
        parse_header(c);
    if (change == 1 && parsed == false)
<<<<<<< HEAD
    {
        parseHeaderLines();
        for (std::map<std::string, std::string>::iterator it = headerMap.begin(); it != headerMap.end(); it++)
            std::cout << (*it).first << " = " << (*it).second << std::endl;
    }
    else if (change == 1 && full == 0)
        parse_body(c);
    return full;
=======
        parseHeaderLines(config);
    else if (change == 1)
        parse_body(c);
    checkContentLength(c.size());
    if (read <= 0)
        return true;
    return false;
>>>>>>> akhalidy
}

int Request::parse_header(std::string c)
{
    size_t pos = 0;
    std::string line(c);
    std::string tmp;
    int i = 0;
<<<<<<< HEAD
   // std::cout << "helqqqq" << std::endl;
=======
>>>>>>> akhalidy
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
<<<<<<< HEAD
                std::cout << save << std::endl;
=======
>>>>>>> akhalidy
                parsefline(save);
                headerPart.push_back(save);
                save = "";
            }
            else
            {
                tmp = line.substr(0, pos);
<<<<<<< HEAD
               // std::cout <<  RED << "|" << tmp << "|"  << RESET << std::endl;
                if (tmp == "\0")
                {
                 //   std::cout << "dasd" << std::endl;
=======
                if (tmp == "\0")
                {
>>>>>>> akhalidy
                    line.erase(0, pos + 2);
                    save = line;
                    change = 1;
                    filePath = ws::fileHandler::createTmp("request_tmp_files/");
<<<<<<< HEAD

                    // std::cout << headerPart.size() << std::endl;
                    // for (int i = 0; i < headerPart.size(); i++)
                    //     std::cout << i << "aaa = " << headerPart[i] << std::endl;
                    // if (save.size() > 0)
                    // {
                    //     int i = 0;
                    //     std::istringstream(headerMap["Content-Length"]) >> i;
                    //      sscanf(headerMap["Content-Length"].c_str(), "%d", &i);
                    //     std::cout << i << std::endl;
                    //     if (save.size() < i )
                    //         ws::fileHandler::write(filePath, save);
                    //save = "";
                    // }
                    break;
                    //return (2);
=======
                    break;
>>>>>>> akhalidy
                }
                save += line;
                headerPart.push_back(save);
                save = "";
            }
            line.erase(0, pos + 2);
        }
        save = line;
    }
<<<<<<< HEAD
    //parseHeaderLines();
    return 0;
}

void Request::parseHeaderLines()
{
    size_t pos = 0;
    for (int i = 1; i < headerPart.size(); i++)
    {
        // if (headerPart[i].find("GET") != std::string::npos)
        // {
        //     headerMap["method"] = "GET";
        //     headerMap["uri"] = headerPart[i].substr(4, headerPart[i].find("HTTP/") - 4);
        //     headerMap["version"] = headerPart[i].substr(headerPart[i].find("HTTP/") + 5, headerPart[i].find("\r\n") - headerPart[i].find("HTTP/") - 5);
        // }
       // std::cout << YELLOW << headerPart[i] << RESET << std::endl;
        if ((pos = headerPart[i].find(":")) != std::string::npos)
            headerMap[headerPart[i].substr(0, pos)] = headerPart[i].substr(pos + 2, headerPart[i].find("\r\n") - pos - 2);
            
    }
    int i = 0;
    // std::cout << headerMap["Host"] << std::endl;
    std::istringstream(headerMap["Content-Length"]) >> i;
    sscanf(headerMap["Content-Length"].c_str(), "%d", &i);
    content_length = i;
=======
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
    // std::cout << " i am here 2" << std::endl;
    checkContentLength(0);
>>>>>>> akhalidy
    parsed = true;
}

int Request::parse_body(std::string c)
{
<<<<<<< HEAD
   //std::string filePath = ws::fileHandler::createTmp("request_tmp_files/");
 // std::cout << "daasad" << c << std::endl;
   // std::cout << " aaa" << std::endl;
=======
>>>>>>> akhalidy
    if (save.size() > 0)
    {
        save += c;
        ws::fileHandler::write(filePath, save);
    }
    else
<<<<<<< HEAD
    {
        //std::cout << "qqqqqqq" << std::endl;
        ws::fileHandler::write(filePath, c);
    }
    save = "";
    // if (ws::fileHandler::getFileSize(filePath) > 100000000)
    //         full = 1;
=======
        ws::fileHandler::write(filePath, c);
    save = "";
>>>>>>> akhalidy
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
<<<<<<< HEAD
        // pos = c.find("\r\n");
        // line = c.substr(0, pos);
=======
>>>>>>> akhalidy
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
<<<<<<< HEAD
=======
        tmpUri = uri;
>>>>>>> akhalidy

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

<<<<<<< HEAD
    // int checkContentLength()
    // {
    
    // }
=======
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
        // else
        //     throw "404";
    }

>>>>>>> akhalidy
