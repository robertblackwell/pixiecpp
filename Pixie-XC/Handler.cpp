//
//  SocketHandler.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//
#include <unistd.h>
#include "Logger.h"
#include "socket_functions.hpp"
#include "Handler.hpp"
#include "BlkSocket.h"
//#include "BlkClient.hpp"

// TODO: need to break this out into a separate class, one for each proxy protocol
// Applies the rules for turning a proxied request into a final request.
// very simple for the BLK protocol
//
void blk_proxy_rules(BlkMessage& in_msg, BlkMessage& out_msg)
{
    out_msg.firstLine = in_msg.firstLine;
    std::string prefix("MITM 8002 : ");
    std::string new_body = prefix + in_msg.body;
    out_msg.body = new_body;
    out_msg.messageLength = (int)new_body.length();

}

Handler::Handler(Protocol _protocol, socket_handle_t socketHandle, int _id)
{
    id = _id;
    socket_fd = socketHandle;
    protocol = _protocol;
    pipelining_supported = true;
}
void Handler::handle()
{
    switch(protocol)
    {
        case Protocol::BLK_PROXY:
            handleBlkProxy();
            break;
        case Protocol::HTTP_PROXY:
            handleHttpProxy();
            break;
        case Protocol::LOOPBACK:
            handleLoopBack();
            break;
    }
}
void Handler::handleHttpProxy()
{
}

void Handler::handleBlkProxy()
{
#ifdef CLINET
    BlkSocket myMessageSocket(socket_fd);
    BlkClient client;

    int count = 0;
    //    LOG(DEBUG) << "SocketHandler id : " << id <<  " socket : " << socket_fd << std::endl;
    
    LOG(DEBUG)  << "Handler starting worker id : " << id
    << " socket: " << socket_fd
    << " count : " << count
    << std::endl;
    
    while(true)
    {
        count++;
        //        LOG(DEBUG)  << "worker id : " << id
        //            << " socket: " << socket_fd
        //            << " count : " << count
        //            << std::endl;
        
        BlkMessage msg;
        int status;
        
        bool gotMessage = myMessageSocket.readMessage(msg, status);
        if( gotMessage && status == BLK_READ_STATUS_OK )
        {
            // process the message and send response

            client.host = "localhost";
            client.port = 8002;
            
            //
            // Try a number of times to connect - BUT dont retry IO operations
            //
            bool   is_connected = false;
            int     retry_count = 0;
            while(
                  ((is_connected  = client.connect(status)) == false)
                   &&
                  (retry_count++ < 3)
                  )
            {
                sleep(2);
            }
                
            if( is_connected )
            {
                blk_proxy_rules(msg, client.requestMessage);
                if( client.executeRequest(status) )
                {
                    bool res = myMessageSocket.writeMessage(client.responseMessage, status);
                }
            }
            client.close();
            if( status != 0 ){
                myMessageSocket.close();
                break;
            }
            if( ! pipelining_supported )
                break;
            
        }
        else if( status == BLK_READ_STATUS_EOF)
        {
            myMessageSocket.close();
            break;
        }
        if(status == BLK_READ_STATUS_PARSE_ERROR)
        {
            myMessageSocket.close();
            break;
        }
        else if( status == BLK_READ_STATUS_IOERROR)
        {
            // close the connection
            break;
        }
    }
    
    LOG(DEBUG)  << "Handler ending worker id : " << id
        << " socket: " << socket_fd
        << " count : " << count
        << std::endl;

#endif
}
void Handler::handleLoopBack()
{
    BlkSocket myMessageSocket(socket_fd);

    int count = 0;
//    LOG(DEBUG) << "SocketHandler id : " << id <<  " socket : " << socket_fd << std::endl;

    while(true)
    {
        count++;
//        LOG(DEBUG)  << "worker id : " << id
//            << " socket: " << socket_fd
//            << " count : " << count
//            << std::endl;

        BlkMessage msg;
        int status;

        bool gotMessage = myMessageSocket.readMessage(msg, status);
        if( gotMessage  )
        {
            // process the message and send response
            int status;
            bool res = myMessageSocket.writeMessage(msg, status);
            std::cout << "Done one " << std::endl;
            
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