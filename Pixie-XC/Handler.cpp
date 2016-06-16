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
#include "Handler.hpp"
#include "HandlerBlk.hpp"


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
            {
                HandlerBlk blkHandler = HandlerBlk(protocol, socket_fd, id);
                blkHandler.handleBlkNormal();
            }
            break;
        case Protocol::HTTP_PROXY:
            {
                handleHttpProxy();
            }
            break;
        case Protocol::LOOPBACK:
            {
                HandlerBlk blkHandler = HandlerBlk(protocol, socket_fd, id);
                blkHandler.handleBlkLoopBack();
            }
            break;
    }
}

#pragma mark - HTTP protocol handlers

void Handler::handleHttpProxy()
{
//    HandlerHttp handler{protocol, socket_fd, id};
//    handler.handle();
}
