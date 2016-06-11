#ifndef THREAD_INC
#define THREAD_INC 1

#include <pthread.h>

class Thread
{
public:
    pthread_t pthread;
    /**
     * Call this to start the thread
     */
    bool start();
    
    /**
     * Override the main method and make it the place where the thread mainline lives
     */
    virtual void main() = 0;
private:
    static void * InternalThreadEntryFunc(void * This);
};
#endif
