#include "socket_functions.hpp"

#ifndef LISTENER
#define LISTENER 1


class Queue;

#include "Thread.h"

class Listener: public Thread
{
    public :

    Queue&          queue;
    int             port;
    socket_handle_t socket_fd;
    
    Listener(Queue& _queue, int _port);
    ~Listener();
    
    void main();
};

//void* Listener_start(void* arg);

#endif