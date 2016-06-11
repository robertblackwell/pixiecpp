#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cassert>

#include "Logger.h"
#include "socket_functions.hpp"
#include "Message.h"
#include "MsgSocket.h"


/**
 * Reads data from a socket and parses the incoming data into a BLKMessage
 *
 * If a message is parsed it is returned in the ref argument msg and function returns true
 * If somethings goes wrong the function returns false and the status indocates the problem
 *
 *  BLK_READ_STATUS_EOF         -   got an EOF and no data
 *  BLK_READ_STATUS_PARSE_ERROR -   the data was not parsed successfully
 *  BLK_READ_STATUS_IOERROR     -   got an io error on the socket
 */
bool MsgSocket::readMessage(Message& msg, int* status)
{
    int buffer_length = 10000;
    char buffer[buffer_length];
    
    Parser parser(msg);
    int sockStatus;
    
    for(;;){
        int n = socket_read_data(socket, buffer, buffer_length, &sockStatus);
        
        std::string buffer_as_string(buffer, n);
        
        if( sockStatus == SOCKET_STATUS_GOOD){
            *status = BLK_READ_STATUS_OK;
            //
            // NOTE - do not return
            //
        }else if(sockStatus == SOCKET_STATUS_EOF){
            *status = BLK_READ_STATUS_EOF;
            return false;
        }else if(sockStatus == SOCKET_STATUS_ERROR){
            *status = BLK_READ_STATUS_IOERROR;
            return false;
        }else{
            assert(false);
        }
        parser.append(buffer, n);
        if( parser.messageComplete ){
            return true;
        }
        if( parser.parseError ){
            *status = BLK_READ_STATUS_PARSE_ERROR;
            return false;
        }
    }
    return true;
}

bool MsgSocket::writeMessage(Message& msg, int& status)
{
    std::string rawMessage("");
    rawMessage += msg.firstLine;
    rawMessage += "\n";
    rawMessage += std::to_string(msg.messageLength);
    rawMessage += "\n";
    rawMessage += msg.body;
    const char* buffer = rawMessage.c_str();
    int   buffer_length = (int)strlen(buffer);
    int sockStatus;
    bool x = socket_write_data(socket, (void*)buffer, buffer_length, &sockStatus);
    
    return true;
}
void MsgSocket::flush()
{
    socket_wait_for_write_flush(socket);
}
void MsgSocket::close()
{
    socket_close(socket);
}
