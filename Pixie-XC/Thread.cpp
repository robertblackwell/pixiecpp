#include "Thread.h"

bool Thread::start()
{
    return (pthread_create(&pthread, NULL, InternalThreadEntryFunc, this) == 0);
}

void * Thread::InternalThreadEntryFunc(void * This)
{
    ((Thread *)This)->main();
    return NULL;
}

