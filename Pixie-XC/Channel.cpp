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

/**
 * --------------------------------------------------------------------------------------------
 */

RawTwoWayChannel::RawTwoWayChannel(int _socketHandleClient, int _socketHandleServer)
                :   socketHandleClient(_socketHandleClient),
                socketHandleServer(_socketHandleServer)

{
}


void RawTwoWayChannel::start()
{
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

}

