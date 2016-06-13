//
//  SocketHandler.hpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef RawTwoWayChannel_hpp
#define RawTwoWayChannel_hpp

#include <stdio.h>
#include "Protocol.h"
#include "BlkMessage.h"
#include "BlkSocket.h"

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
    void startChannel(bool* terminateFlag);
    void run(int count, std::string message, bool* terminateFlag);
    
};


/**
 * --------------------------------------------------------------------------------------------
 */

class RawOneWayThreadedChannel: public Thread, public RawOneWayChannel
{
public:
    pthread_t   thread;
    bool        *terminateFlag;
    
    RawOneWayThreadedChannel(int _readSocket, int _writeSocket);
    void startThread(bool*  _terminateFlag);
    void main();
};


/**
 * --------------------------------------------------------------------------------------------
 */


class RawTwoWayChannel
{
public:
    int                         socketHandleClient;
    RawOneWayThreadedChannel    channelUpStream;
    
    int                         socketHandleServer;
    RawOneWayChannel            channelDownStream;
    
    RawTwoWayChannel(int _socketHandleClient, int _socketHandleServer);
    void start();
    
private:
    
};




#endif /* SocketHandler_hpp */
