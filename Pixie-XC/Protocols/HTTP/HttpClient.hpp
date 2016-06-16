

#ifndef ClientHttp_hpp
#define ClientHttp_hpp
#
#include <stdio.h>
#include <string>
#include "HttpMessage.h"
#include "HttpSocket.h"

class HttpClient
{
public:
    std::string             host;
    std::string             url;
    int                     port;
    int                     socket;
    
    HttpMessage              requestMessage{};
    HttpMessage              responseMessage{};
    HttpSocket               messageSocket{};
    
    HttpClient(std::string _host, std::string _url, int _port);
    HttpClient(std::string _host, std::string _url, int _port, HttpMessage& _requestMessage);
    
    bool    connect(int& status);
    
    bool    executeRequest(int& status);
    
    void close();
    ~HttpClient();
};


#endif 
