//
//  Client.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 6/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//
#include <iostream>
#include <unistd.h>
#include "Logger.h"
#include "socket_functions.hpp"
#include "HttpMessage.h"
#include "HttpSocket.h"
#include "HttpClient.hpp"


HttpClient::HttpClient(std::string _host, std::string _url, int _port) :  host(_host),
                                                                        url(_url),
                                                                        port(_port)
{
    std::cout <<  " got here  " << std::endl;
}


HttpClient::HttpClient(std::string _host, std::string _url, int _port, HttpMessage& _requestMessage) :    host(_host),
                                                                                    url(_url),
                                                                                    port(_port),
                                                                                    requestMessage(_requestMessage)
{
    std::cout <<  " got here  " << std::endl;
}


bool HttpClient::connect(int& status)
{
    char*   _host = (char*)host.c_str();
    int     _port = port;
    int     local_status = 0;
    
    socket_handle_t sock = socket_connect_host_port(_host, _port, &local_status);
    if( sock && local_status == SOCKET_STATUS_GOOD )
    {
        socket = sock;
        messageSocket.socket = sock;
    }
    else
    {
        int c, hm;
        char* m;
        c = asprintf(&m, "HttpClient::execute connect FAILED port: %d sock : %d \n", port, messageSocket.socket);
        hm = (int)write(1, m, (int)strlen(m));
        free(m);
    }
    status = local_status;
    return (local_status == 0);
}

bool    HttpClient::executeRequest(int& status)
{
//    int     status;

    messageSocket.writeMessage(requestMessage, status);
    if( status == BLK_WRITE_STATUS_OK)
    {
        messageSocket.readMessage(responseMessage, status);
    }
    return (status == 0);

}


void HttpClient::close()
{
    LOG(DEBUG) << "HttpClient::close - socket : "<< socket << std::endl;
    socket_close(socket);
}

HttpClient::~HttpClient()
{
    
}