#ifndef WORKER
#define WORKER 1
#include "Thread.h"

class Queue;

class Worker : public Thread
{
    public :
    bool    active;
    int     active_socket;
    Queue&  queue;
    int     id;
    Worker(Queue& _queue, int _id);
    ~Worker();
    
    void main();
    
};

typedef std::shared_ptr<Worker> WorkerSharedPtr;
typedef std::vector<WorkerSharedPtr> WorkerVector;

#endif