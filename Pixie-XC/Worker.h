#ifndef WORKER
#define WORKER 1
#include "Thread.h"
#include "Protocol.h"

class Queue;

class Worker : public Thread
{
    public :
    bool        active;
    int         active_socket;
    Protocol    protocol;
    Queue&      queue;
    int         id;
    Worker(Protocol protocol, Queue& _queue, int _id);
    ~Worker();
    
    void main();
    
};

typedef std::shared_ptr<Worker> WorkerSharedPtr;
typedef std::vector<WorkerSharedPtr> WorkerVector;

#endif