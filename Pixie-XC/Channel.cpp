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
#include "Channel.hpp"
#include "twc.h"

#pragma mark - implementation of raw channel
/**
 * --------------------------------------------------------------------------------------------
 */

RawOneWayChannel::RawOneWayChannel(int _readSocket, int _writeSocket)
    : readSocket(_readSocket), writeSocket(_writeSocket)
{
    
}
void RawOneWayChannel::startChannel(bool* terminateFlag)
{
    run(10, "RawOneWayChannel", terminateFlag);
}

//
// This is where the real work is to be done - a name that can be used in
// both derived classes. The use of Thread class
// makes the names "start" and "main" unavailable
//
void RawOneWayChannel:: run(int count, std::string message, bool *terminateFlag)
{
    int buffer_length = 10000;
    char buffer[buffer_length];
    int sockStatus;
    int retryCount = 0;
    
    int status;
    
    for(;;){
        if( *terminateFlag )
            break;
        
        LOG(DEBUG) << "channel abuot to read  " << message << std::endl;
    
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
                LOG(ERROR) << "EAGAIN ABORT after rettry on read channel rfd:  "<< readSocket
                << " wfd: " << writeSocket
                << "msg: " << message <<  " status: " << sockStatus << std::endl;
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
    *terminateFlag= true;
    return;
    for(int i = 0; i < count; i++) {
        LOG(ERROR) << message << i << std::endl;
        sleep(1);
    }
    
}

/**
 * --------------------------------------------------------------------------------------------
 */


RawOneWayThreadedChannel::RawOneWayThreadedChannel(int _readSocket, int _writeSocket)
                                        :RawOneWayChannel(_readSocket, _writeSocket)
{
    
}
void RawOneWayThreadedChannel::main()
{
    run(10, "RawOneWayThreadedChannel", this->terminateFlag);
//    for(int i = 0; i < 20; i++){
//        LOG(ERROR) << "threaded channel i:  " << i << std::endl;
//        sleep(1);
//    }
    
}
void RawOneWayThreadedChannel::startThread(bool* _terminateFlag)
{
    this->terminateFlag = _terminateFlag;
    bool w = start();
}


/**
 * --------------------------------------------------------------------------------------------
 */

RawTwoWayChannel::RawTwoWayChannel(int _socketHandleClient, int _socketHandleServer)
                :   socketHandleClient(_socketHandleClient),
                socketHandleServer(_socketHandleServer),
                channelUpStream{_socketHandleClient, _socketHandleServer},
                channelDownStream{_socketHandleServer, _socketHandleClient}

{
}


void RawTwoWayChannel::start()
{
#define GGGG
#ifdef GGGG
    int     status;
    two_way_channel_t   twc;
    
    socket_set_non_blocking(socketHandleServer);
    socket_set_non_blocking(socketHandleClient);

    twc_init(&twc, socketHandleClient, socketHandleServer, 10000);
    twc_run(&twc, &status);
    twc_clean(&twc);
    
    socket_set_blocking(socketHandleServer);
    socket_set_blocking(socketHandleClient);

    socket_close(socketHandleClient);
    socket_close(socketHandleServer);

#else
    bool    terminateFlag;
    
    channelUpStream.startThread(&terminateFlag);
    channelDownStream.startChannel(&terminateFlag);
    socket_close(socketHandleClient);
//    socket_close(socketHandleServer);
    
    LOG(DEBUG) << "after starting both channels   " <<  std::endl;
    
    pthread_join(channelUpStream.pthread, NULL);

    LOG(DEBUG) << "after waiting for sub thread   " <<  std::endl;
#endif
}

