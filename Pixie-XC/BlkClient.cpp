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
#include "BlkMessage.h"
#include "BlkSocket.h"
#include "BlkClient.hpp"


BlkClient::BlkClient()
{
    host = "";
    url = "";
    port = 0;
}

BlkClient::BlkClient(std::string _url, int _port)
{
    url = _url;
    port = _port;
}

bool BlkClient::connect(int& status)
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
        c = asprintf(&m, "BlkClient::execute connect FAILED port: %d sock : %d \n", port, messageSocket.socket);
        hm = (int)write(1, m, (int)strlen(m));
        free(m);
    }
    status = local_status;
    return (local_status == 0);
}

void BlkClient::setBody(std::string& body)
{
    requestMessage.firstLine = "START";
    requestMessage.destination_port =  port ;
    requestMessage.request_verb = std::string("NORMAL");
    requestMessage.body = body;
    requestMessage.messageLength = (int)body.length();
}
void BlkClient::formatRequest()
{
    return;
//    requestMessage.firstLine = std::string{"START"};
//    requestMessage.destination = std::to_string(port);
//    requestMessage.request_verb = std::string("NORMAL)
//    requestMessage.messageLength = 10;
//    requestMessage.body = "0123456789";

    requestMessage.firstLine = std::string{"START"};
    requestMessage.messageLength = 10;
    requestMessage.body = "0123456789";
}

bool    BlkClient::executeRequest(int& status)
{
//    int     status;

    messageSocket.writeMessage(requestMessage, status);
    if( status == BLK_WRITE_STATUS_OK)
    {
        messageSocket.readMessage(responseMessage, status);
    }
    return (status == 0);

}


void BlkClient::close()
{
    LOG(DEBUG) << "BlkClient::close - socket : "<< socket << std::endl;
    socket_close(socket);
}

BlkClient::~BlkClient()
{
    
}