//
//  main.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 12/27/15.
//  Copyright © 2015 Blackwellapps. All rights reserved.
//
#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>

#include "socket_functions.hpp"
#include "Logger.h"
#include "Exception.hpp"
#include <stdexcept>

#include "Queue.h"
#include "Listener.h"

Listener::Listener(Queue& _queue, int _port): queue(_queue)
{
    queue =         _queue;
    port =          _port;
}
Listener::~Listener(){
    
}
void Listener::main()
{
    try
    {
        struct sockaddr_in peername;
        unsigned int addr_length = (unsigned int)sizeof(peername);
        socket_fd = socket_create_listener_on_port(port);
        for(;;)
        {
            int sock2 = accept(socket_fd, (struct sockaddr*)&peername, &addr_length);
            if( sock2 <= 0 )
            {
                std::cout << "Listener thread :: accept failed terminating " << std::endl;
                throw std::runtime_error("Listener THread :: accept failed ");
                break;
            }
            queue.add(sock2);
        }
    }
    catch(std::runtime_error e)
    {
        std::cerr << "Listener Thread Terminated abnromally with runtime_error" << std::endl;
    }
    catch (...)
    {
        std::cerr << std::endl << "Uncaught exception in Listener" << std::endl;
    }
}

