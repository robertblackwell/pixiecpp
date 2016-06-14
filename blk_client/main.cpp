#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include "Exception.hpp"
#include "socket_functions.hpp"
#include "Handler.hpp"
#include "Protocol.h"
#include "BlkClient.hpp"

#include "Logger.h"
INITIALIZE_EASYLOGGINGPP

#define     NBR_THREADS 100
#define     NBR_REQUESTS_PER_THREAD 100
void* worker(void* args){
    int status;
//    LOG(INFO) << "Worker started" << (long)args << std::endl;
    for(int i = 0; i < NBR_REQUESTS_PER_THREAD; i++)
    {
        
        BlkMessage msg{8001, std::string("NORMAL"), std::string("00119922883377446655")};
        
        BlkClient client{"localhost", "a_url", 8001,  msg};
        if( ! client.connect(status) ){
            LOG(ERROR) << "Connect for worker : " << i << " Failed status " << status  << std::endl;
            break;
        }
        if( !client.executeRequest(status) ){
            LOG(ERROR) << "executeRequest for  worker : " << i << " Failed status "<< status << std::endl;
            break;
        }
        client.close();
    }
    
    return NULL;
}

int main(int argc, const char * argv[])
{
    
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    //    defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Global, el::ConfigurationType::Format, "%level|%func[%fbase:%line] : %msg");
    el::Loggers::reconfigureAllLoggers(defaultConf);
    
    std::cout << "Requester starting " << std::endl;
    pthread_t threads[NBR_THREADS];
    auto begin = std::chrono::high_resolution_clock::now();
    try
    {
        
        for( int i = 0; i < NBR_THREADS; i++){
            pthread_t* th = &(threads[i]);
            int rc =  pthread_create(th, NULL, worker, (void*)(long)i);
            if( rc != 0 ){
                LOG(ERROR) << " UNHANDLED ERROR ******* "<< std::endl;
                
            }
        }
        
    }
    catch(Exception e)
    {
        std::cerr << "Abnornal Termination" <<  e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Abnornal Termination" << std::endl;
    }
    for(int i = 0 ; i < NBR_THREADS; i++)
    {
        pthread_t* th = &(threads[i]);
        int rc = pthread_join(*th, NULL);
        if( rc != 0 ){
            LOG(ERROR) << " UNHANDLED ERROR ******* "<< std::endl;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    float duration = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / 1000000000.0);
    float average = (float)duration/ ( 1.0* (float)NBR_THREADS );
    LOG(INFO) << "All Requests complete Elapsed time " << (long)(duration) << "average: " <<  (average) << std::endl;
    return 0;
}
