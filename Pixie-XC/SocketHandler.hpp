//
//  SocketHandler.hpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef SocketHandler_hpp
#define SocketHandler_hpp

#include <stdio.h>

class SocketHandler
{
public:
    socket_handle_t socket_fd;
    int             id;
    SocketHandler(socket_handle_t socket_handle, int _id);
    void handle();
};

#endif /* SocketHandler_hpp */
