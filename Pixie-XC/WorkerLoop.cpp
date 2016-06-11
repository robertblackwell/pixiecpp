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
#include "WorkerLoop.h"

WorkerLoop::WorkerLoop(Queue& _queue, int _id): queue(_queue)
{
    active_socket = 0;
    active  = false;
    id = _id;
}
WorkerLoop::~WorkerLoop(){
    
}

void WorkerLoop::main()
{
//    Worker* w = (Worker*)arg;
//    Queue*  queue = w->queue;
//    int id = w->id;
//    delete w;
    
    int count = 0;
    
    while(true)
    {
        active = false;
        int mySocketHandle = queue.remove();
        active_socket = (int) mySocketHandle;
        sleep(20);
        count++;
        if(count == 3){
            break;
        }
    }
}

