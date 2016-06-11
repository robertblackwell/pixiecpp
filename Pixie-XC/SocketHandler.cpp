//
//  SocketHandler.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//
#include "Logger.h"
#include "socket_functions.hpp"
#include "SocketHandler.hpp"
#include "BlkSocket.h"

SocketHandler::SocketHandler(socket_handle_t socketHandle, int _id)
{
    id = _id;
    socket_fd = socketHandle;
}
void SocketHandler::handle()
{
    BlkSocket myMessageSocket(socket_fd);

    int count = 0;
    LOG(DEBUG) << "SocketHandler id : " << id <<  " socket : " << socket_fd << std::endl;

    while(true)
    {
        count++;
        LOG(DEBUG)  << "worker id : "
        << id << " socket: "
        << socket_fd
        << " count : "
        << count
        << std::endl;
        BlkMessage msg;
        int status;

        bool gotMessage = myMessageSocket.readMessage(msg, &status);
        if( gotMessage  )
        {
            // process the message and send response
            int status;
            bool res = myMessageSocket.writeMessage(msg, status);
            
        }
        else if( status == BLK_READ_STATUS_EOF)
        {
            // orderly close the connection
            myMessageSocket.flush();
            break;
        }
        if(status == BLK_READ_STATUS_PARSE_ERROR)
        {
            // close the connection, possibly after sending an error response
            myMessageSocket.flush();
            break;
        }
        else if( status == BLK_READ_STATUS_IOERROR)
        {
            // close the connection
            break;
        }
    }
}