#ifndef WORKER
#define WORKER 1
#include "Thread.h"
class Queue;

class WorkerLoop : public Thread
{
    public :
    bool    active;
    int     active_socket;
    Queue&  queue;
    int     id;
    WorkerLoop(Queue& _queue, int _id);
    ~WorkerLoop();
    
    void main();
    
};

#endif