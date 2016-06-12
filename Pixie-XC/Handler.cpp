//
//  SocketHandler.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//
#include <unistd.h>
#include <cassert>
#include <pthread.h>
#include <errno.h>
#include "Logger.h"
#include "socket_functions.hpp"
#include "Thread.h"
#include "Handler.hpp"
#include "BlkSocket.h"
#include "BlkClient.hpp"

#pragma mark - implementation of raw channel
/**
 * --------------------------------------------------------------------------------------------
 */
class RawOneWayChannel
{
public:
    int         readSocket;
    int         writeSocket;
    int         exitStatus;
    
    RawOneWayChannel(int _readSocket, int writeSocket);
    void startChannel();
    void run(int count, std::string message);
    
};

RawOneWayChannel::RawOneWayChannel(int _readSocket, int _writeSocket)
    : readSocket(_readSocket), writeSocket(_writeSocket)
{
    
}
void RawOneWayChannel::startChannel()
{
    run(10, "RawOneWayChannel");
}

//
// This is where the real work is to be done - a name that can be used in
// both derived classes. The use of Thread class
// makes the names "start" and "main" unavailable
//
void RawOneWayChannel:: run(int count, std::string message)
{
    int buffer_length = 10000;
    char buffer[buffer_length];
    int sockStatus;
    int retryCount = 0;
    
    int status;
    
    for(;;){
        LOG(DEBUG) << "channel abut to read  " << message << std::endl;
        
        int n = socket_read_data( readSocket, buffer, buffer_length, &sockStatus);
        
        LOG(DEBUG) << "channel back from read "<< message <<  " n: " << n << " status " << sockStatus << std::endl;
        
        if( n > 0 && sockStatus == SOCKET_STATUS_GOOD){
            retryCount = 0;
            std::string buffer_as_string(buffer, n);

            socket_write_data(writeSocket, buffer, n, &sockStatus);

            LOG(DEBUG) << "channel back from write "<< message <<  " status " << sockStatus << std::endl;
        }else if( n < 0 && sockStatus == SOCKET_STATUS_EAGAIN ){
            retryCount ++;
            LOG(ERROR) << "EAGAIN on read channel rfd:  "<< readSocket
            << " wfd: " << writeSocket
            << "msg: " << message <<  " status: " << sockStatus << std::endl;
            if( retryCount < 3){
                sleep(2);
            }else{
                sockStatus = SOCKET_STATUS_ERROR;
            }
            
        }
        if( sockStatus == SOCKET_STATUS_GOOD){
            status = BLK_READ_STATUS_OK;
        }else if( sockStatus == SOCKET_STATUS_EAGAIN   ) {
            
        }else if( (sockStatus == SOCKET_STATUS_EOF) || (sockStatus == SOCKET_STATUS_ERROR) ){
            status = BLK_READ_STATUS_IOERROR;
            LOG(ERROR) << "channel rfd:  "<< readSocket
                << " wfd: " << writeSocket
                << "msg: " << message <<  " status: " << sockStatus << std::endl;
            break;
        }else{
            assert(false);
        }
    }
    return;
    for(int i = 0; i < count; i++) {
        LOG(ERROR) << message << i << std::endl;
        sleep(1);
    }
    
}

/**
 * --------------------------------------------------------------------------------------------
 */

class RawOneWayThreadedChannel: public Thread, public RawOneWayChannel
{
public:
    pthread_t   thread;
    
    RawOneWayThreadedChannel(int _readSocket, int _writeSocket);
    void startThread();
    void main();
};

RawOneWayThreadedChannel::RawOneWayThreadedChannel(int _readSocket, int _writeSocket)
                                        :RawOneWayChannel(_readSocket, _writeSocket)
{
    
}
void RawOneWayThreadedChannel::main()
{
    run(10, "RawOneWayThreadedChannel");
//    for(int i = 0; i < 20; i++){
//        LOG(ERROR) << "threaded channel i:  " << i << std::endl;
//        sleep(1);
//    }
    
}
void RawOneWayThreadedChannel::startThread()
{
    bool w = start();
}


/**
 * --------------------------------------------------------------------------------------------
 */


class RawTwoWayChannel
{
public:
    int                         socketHandleOne;
    RawOneWayChannel            channelOne;
    
    int                         socketHandleTwo;
    RawOneWayThreadedChannel    channelTwo;
    
    RawTwoWayChannel(int _socketHandleOne, int _socketHandleTwo);
    void start();
    
private:
    
};

RawTwoWayChannel::RawTwoWayChannel(int _socketHandleOne, int _socketHandleTwo)
                :   socketHandleOne(_socketHandleOne),
                socketHandleTwo(_socketHandleTwo),
                channelOne{_socketHandleOne, _socketHandleTwo},
                channelTwo{_socketHandleTwo, _socketHandleOne}

{
}


void RawTwoWayChannel::start()
{
    channelTwo.startThread();
    channelOne.startChannel();
    socket_close(socketHandleOne);
    socket_close(socketHandleTwo);
    
    LOG(DEBUG) << "after starting both channels   " <<  std::endl;
    
    // wait for the subordinate thread to complete
    pthread_join(channelTwo.pthread, NULL);
    LOG(DEBUG) << "after waiting for sub thread   " <<  std::endl;
    
}

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

#pragma mark - HTTP protocol handlers

void Handler::handleHttpProxy()
{
}

#pragma mark - Blk protocol proxy handlers
void Handler::handleBlkProxy()
{
    BlkSocket myMessageSocket(socket_fd);
    messageSocket.socket = socket_fd;
    BlkMessage msg;
    int status;
    LOG(DEBUG)  << "Handler starting worker id : " << id << " socket: " << socket_fd << std::endl;
    
    bool gotMessage = myMessageSocket.readMessage(msg, status);
    if( gotMessage && status == BLK_READ_STATUS_OK )
    {
        if( msg.isTunnelRequest() )
        {
            tunnelBlkProxy(msg);
        }
        else
        {
            normalBlkProxy(msg);
        }

    }
    else if( ( status == BLK_READ_STATUS_EOF)
        || (status == BLK_READ_STATUS_PARSE_ERROR)
        || ( status == BLK_READ_STATUS_IOERROR))    {
        
        myMessageSocket.close();
    }
    LOG(DEBUG)  << "Handler ending worker id : " << id << " socket: " << socket_fd<< std::endl;
}

void Handler::tunnelBlkProxy(BlkMessage& msg)
{
    LOG(DEBUG) << "got a tunnel request" << std::endl;
    int status;
    char*   host = "localhost";
    int upstreamPort = msg.destination_port;
    int upstreamRawSocket = socket_connect_host_port(host,upstreamPort,&status);
    
    socket_set_timeout(upstreamRawSocket, 5);
    socket_set_timeout(socket_fd, 5);
    
    socket_set_blocking(upstreamRawSocket);
    socket_set_blocking(socket_fd);
    
    bool blk1 = socket_is_blocking(upstreamRawSocket);
    bool blk2 = socket_is_blocking(socket_fd);
    
    LOG(DEBUG) << "XXtunnel request connection upstreamSocket: " << upstreamRawSocket << " status: " << status << std::endl;
    LOG(DEBUG) << "XXtunnel request connection downStreamsocket: " << socket_fd << " status: " << status << std::endl;
    BlkMessage okMessage{8001, "TUNNEL", "OK TO TUNNEL"};
    BlkSocket myMessageSocket(socket_fd);
    
    if( status == SOCKET_STATUS_GOOD ){
        
        myMessageSocket.writeMessage(okMessage, status);
        if( status == BLK_WRITE_STATUS_OK){    
            RawTwoWayChannel  channel{socket_fd, upstreamRawSocket};
            channel.start();
        }else{
            LOG(ERROR) << "write OK message failed upSock: " << upstreamRawSocket
            << " downStreamsocket: " << socket_fd << " status: " << status << std::endl;
            
        }
        
    }else{
        LOG(ERROR) << "connect upstream failed upSock: " << upstreamRawSocket
        << " downStreamsocket: " << socket_fd << " status: " << status << std::endl;
        exit(1);
    }
//    socket_close(upstreamRawSocket);
//    socket_close(socket_fd);
}

void Handler::normalBlkProxy(BlkMessage& msg)
{
    // process the message and send response
    int status;
    std::string host = std::string("localhost");
    int 		port = msg.destination_port;
    std::string	url	 = std::string("dont care");
    for(;;)
    {
        BlkClient client{host, url, port, msg};
        
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
            LOG(ERROR) << "Client Failed to connect with server" << std::endl;
            sleep(2);
        }
        
        if( is_connected )
        {
            blk_proxy_rules(msg, client.requestMessage);
            if( client.executeRequest(status) )
            {
                bool res = this->messageSocket.writeMessage(client.responseMessage, status);
                if( ! res ){
                    LOG(ERROR) << "UNHANDLED ERROR " << std::endl;
                }
            }
        }
        client.close();
        if( status != 0 ){
            this->messageSocket.close();
            break;
        }
        bool gotMessage = this->messageSocket.readMessage(msg, status);
        if( gotMessage && status == BLK_READ_STATUS_OK  && pipelining_supported)
        {
        }
        else
        {
            this->messageSocket.close();
            break;
        }
        
    }
    
}

#pragma mark - Blk protocol loopback handlers

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
            if( ! res ){
                LOG(ERROR) << " UNHANDLED ERROR ******* "<< std::endl;
            }
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