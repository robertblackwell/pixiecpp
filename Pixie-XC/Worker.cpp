#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cassert>
#include <unistd.h>

#include "Logger.h"
#include "socket_functions.hpp"
#include "Queue.h"
#include "Worker.h"
#include "Statistics.h"
#include "Protocol.h"
#include "Handler.hpp"

Worker::Worker(Protocol _protocol, Queue& _queue, int _id): queue(_queue)
{
    protocol = _protocol;
    active_socket = 0;
    active  = false;
    id = _id;
}
Worker::~Worker()
{
    std::cout << "Worker::~Worker called" << std::endl;
}

void Worker::main()
{
    while(true)
    {
        active = false;
        int mySocketHandle = queue.remove();
        active_socket = (int) mySocketHandle;
        try
        {
            active = true;
            Handler handler{protocol, mySocketHandle, id};
            handler.handle();
            active = false;
        }
        catch (std::exception e)
        {
            std::cout << "Worker - exception from socket handler" << std::endl;
        }
        //close(mySocketHandle);
    }
    
}

