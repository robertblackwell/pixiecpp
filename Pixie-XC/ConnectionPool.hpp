//
//  ConnectionPool.hpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 6/12/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef ConnectionPool_hpp
#define ConnectionPool_hpp

#include <stdio.h>
#include "Queue.h"

class Connection
{
public:
    std::string     host;
    int             port;
    int             socket;
};

class ConnectionPool
{
public:
//    static  Queue           availableQueue;
//    static  Connection      connections[100];
    static  void            initialize();
    static  int             acquire(int port);
    static  void            release(int socket);
};


#endif /* ConnectionPool_hpp */
