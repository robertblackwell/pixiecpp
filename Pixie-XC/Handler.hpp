//
//  SocketHandler.hpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef Handler_hpp
#define Handler_hpp

#include <stdio.h>
#include "Protocol.h"

class Handler
{
public:
    Protocol        protocol;
    bool            pipelining_supported;
    socket_handle_t socket_fd;
    int             id;
    Handler(Protocol protocol, socket_handle_t socket_handle, int _id);
    void handle();
private:
    void handleLoopBack();
    void handleBlkProxy();
    void handleHttpProxy();
};

#endif /* SocketHandler_hpp */
