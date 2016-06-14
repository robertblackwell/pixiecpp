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



class RawTwoWayChannel
{
public:
    int                         socketHandleClient;    
    int                         socketHandleServer;
    
    RawTwoWayChannel(int _socketHandleClient, int _socketHandleServer);
    void start();
    
private:
    
};




#endif /* SocketHandler_hpp */
