#include "Thread.h"
#include "Worker.h"

#ifndef MONITOR_INC
#define MONITOR_INC 1
class Queue;
class Worker;

class Monitor: public Thread
{
    public :
    //
    // vector of shared pointers to Worker objects
    //
    WorkerVector&     workers;
    
    unsigned    count;
    
    Monitor(std::vector<std::shared_ptr<Worker>>& _workers, int _count);
    ~Monitor();
    
    void main();
    void scanWorkerTable();
};


#endif