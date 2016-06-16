//
//  ConnectionPool.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 6/12/16.
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

#include "ConnectionPool.hpp"

static  Queue           availableQueue;
static  Connection      connections[100];


void ConnectionPool::initialize()
{
    int status;
    int index = 0;
    char* host = (char*)"localhost";
    for(int i = 0; i < 10; i ++)
    {
        int sock = socket_connect_host_port(host, 8002, &status);
        if( sock > 0){
            Connection con;
            con.socket = sock;
            con.port = 8002;
            con.host = std::string("localhost");
            connections[index] = con;
            availableQueue.add(sock);
            index++;
        }
    }
}

int ConnectionPool::acquire(int port)
{
    assert(port == 8002);
    int temp = availableQueue.remove();
    return temp;
}
void ConnectionPool::release(int socket)
{
    availableQueue.add(socket);
}
