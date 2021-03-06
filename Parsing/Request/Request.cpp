/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 14:33:10 by mokhames          #+#    #+#             */
/*   Updated: 2022/06/30 18:42:05 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
// #include "fileHandler.hpp"
#include "../../Includes/networking.hpp"
#include "../Config/Config.hpp"  
#include "../Config/Server.hpp"  
#include "../Config/Location.hpp"



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
    this->version = "HTTP/1.1";
    hostIp = "127.0.0.1";
    hostPort = 0;
    this->content_type = "";
    this->contentLength = 0;
    this->read = 0;
    this->parsed = false;
    fchuncked = 0;
    save = "";
    query = "";
    change = 0;
    full = 0;
	cgi_ptr = NULL;
}

Request::Request(std::string file)
{
    requestFile = file;
}
Request::~Request()
{
	if (cgi_ptr)
	{
		delete cgi_ptr;
		cgi_ptr = NULL;
	}
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
        query = c.query;
        content_type = c.content_type;
        contentLength = c.contentLength;
        parsed = c.parsed;
    }
    return *this;
}


// ! /////////////////////// file_Readers //////////////////////

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
        parseHeaderLines(config);
    if (change == 1)
        parse_body(c);
    checkContentLength(0);
    if (read <= 0 && change == 1)
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
    for (size_t i = 1; i < headerPart.size(); i++)
    {
        if ((pos = headerPart[i].find(":")) != std::string::npos)
        {
            if (headerPart[i].substr(0, pos) == "Cookie")
                cookies.push_back(headerPart[i].substr(pos + 2, headerPart[i].find("\r\n") - pos - 2));
            else
                headerMap[headerPart[i].substr(0, pos)] = headerPart[i].substr(pos + 2, headerPart[i].find("\r\n") - pos - 2); 
        }
    }
    getRightServer(config);
    fetchContentLength();
    getRightLocation();
    checkTransferEncoding();
    checkContentLength(0);
    parsed = true;
}

int Request::parse_body(std::string c)
{
    if (save.size() > 0)
    {
        ws::fileHandler::write(filePath, save);
        read -= save.size();
    }
    else
    {
        ws::fileHandler::write(filePath, c);
        read -= c.size();
    }
    save = "";
    return 0;
}

 // ! /////////////////////// parse header //////////////////

 void Request::print_header()
 {
     for (size_t i = 0; i < headerPart.size(); i++)
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
        pos = line.find(" ");
        uri = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(" ");
        version = line.substr(0, pos);
        line.erase(0, pos + 1);
        main_error_check();
        parseUri();
        tmpUri = uri;

}

void Request::parseUri()
{
    if (size_t pos = uri.find("?") != std::string::npos)
    {
        
        query = uri.substr(uri.find("?") + 1, uri.size() - pos);
        uri.erase(uri.find("?"), uri.size());
    }
}
void Request::parseCookies()
{
    if (cookies.size())
    {
        for (size_t i = 0; i < cookies.size(); i++)
        {
            std::string cookie = cookies[i];
            size_t pos = 0;
            while ((pos = cookie.find(";")) != std::string::npos)
            {
                std::string tmp = cookie.substr(0, pos);
                if (size_t pos1 = tmp.find("=") != std::string::npos)
                {
                    std::string key = tmp.substr(0, pos1);
                    std::string value = tmp.substr(pos1 + 1, tmp.size());
                    CookieMap[key] = value;
                }
                cookie.erase(0, pos + 1);
            }
        }
    }
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
            throw "400";
           
        }
        return (200);
    }

    int Request::checkURI()
    {
        std::string allowedchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
        for (size_t i = 0; i < uri.size(); i++)
        {
            if (allowedchars.find(uri[i]) == std::string::npos)
                throw "409";
        }
        if (uri.size() > 2048)
            throw "414";
        return (200);
    }

    int Request::checkVersion()
    {
        if (version != "HTTP/1.1" && version != "HTTP/1.0")
        {
            throw "400";
            return (400); // 505 HTTP Version Not Supported
        }
        return (200);
    }

    void Request::checkContentLength(int a)
    {
        if ((contentLength > (long long)location.getClientMaxBodySize() && method != "GET")
            || contentLength < 0)
        {
            throw "413";
        }
        read -= a;
    }
    void Request::checkTransferEncoding()
    {
        if (headerMap.find("Transfer-Encoding") != headerMap.end() && headerMap["Transfer-Encoding"] != "chunked")
            throw "501";
        if ((headerMap.find("Transfer-Encoding") == headerMap.end()) && (headerMap.find("Content-Length") == headerMap.end()) && method == "POST")
            throw "411";
        if (headerMap.find("Content-Length") != headerMap.end() && headerMap["Content-Length"] != "0")
        {
            if (!ws::fileHandler::checkIfExist("./tmp"))
                mkdir("./tmp",0777);
            filePath = ws::fileHandler::createTmp("./tmp");
        }
    }
    // ! ///////////////////////clear  //////////////////
    void Request::clear()
    {
        Server a;
        Location b;
        headerPart.clear();
        headerMap.clear();
        server = a;
        location = b;
        save = "";
        parsed = false;
        method = "";
        hostIp = "127.0.0.1";
        hostPort = 0;
        uri = "";
        version = "";
        fchuncked = 0;
        change = 0;
		ws::fileHandler::removeFile(filePath);
        filePath = "";
		if (cgi_ptr)
		{
			delete cgi_ptr;
			cgi_ptr = NULL;
		}
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
        if (headerMap.find("Host") == headerMap.end())
            throw "400";
        hostIp = headerMap["Host"].substr(0, headerMap["Host"].find(":"));
        if (headerMap["Host"].find(":") != std::string::npos)
            hostPort = atoi(headerMap["Host"].substr(headerMap["Host"].find(":") + 1, headerMap["Host"].size()).c_str());
     }

    void  Request::getRightServer(Config config)
    {
        fetchHost();
        try {
        server = config.getServer(hostPort, hostIp);
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    int  Request::getRightLocation()
    {
        size_t pos = 0;
        std::string clone(tmpUri);
        int n = server.getLocation().size();
        for (int i = 0; i < n; i++) {
            if (server.getLocation()[i].getLocation_match() == tmpUri) {
                location = server.getLocation()[i];
                return 1;
            }
        }
        pos = clone.find_last_of("/");
        if (pos != std::string::npos)
            clone.erase(pos, std::string::npos);
        tmpUri = clone;
        if (pos != 0 && pos != std::string::npos)
            getRightLocation();
        return 0;
    }