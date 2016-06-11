//
//  main.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 12/27/15.
//  Copyright © 2015 Blackwellapps. All rights reserved.
//
#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <array>

#include "Logger.h"
#include "Protocol.h"
#include "Queue.h"
#include "Worker.h"
#include "WorkerLoop.h"
#include "Listener.h"
#include "Monitor.h"
#include "Server.h"

//
// TODO: 3. !!HIGH PRIORITY - need to add retry loginc to all (not all just connection)socket operations
// TODO: 1. upgrade test harness - multiple servers each on separate port, separate client for each server
// TODO: 2. upgrade test harness - clients need to check the returned message and verify it is as expected
// TODO: still has a bug when client can send more than one message-hypothesis -- its a problem with the server
// TODO: better buffer management - BlkMessage needs to have a ref/ptr to body not value definition
// TODO: dispatcher to allow varying number of worker tasks
// TODO: connection pooling for connections to server
// TODO: better statistics
// TODO: pass through handler for situations where dont want to parse messages - preparation for SSL
// TODO: how will SSL fit in
//

Server::Server(Protocol _protocol, unsigned _nbr_workers)
{
    protocol = _protocol;
    nbr_workers = _nbr_workers;
}

void Server::listen(int _port)
{
//    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%level|%func[%fbase:%line] : %msg");
  
    port = _port;

    LOG(INFO) << "Hello, World!\n";
    
    Queue  queue{};
    //
    // Start the listener to accepts connections from clients
    //
    Listener listener{queue,port};
    listener.start();

    //
    // Prepare a place to record the worker threads we start
    //
    WorkerVector worker_v;
    bool rc;
    
    //
    // Start the worker threads
    //
    for(int i = 0; i < nbr_workers; i++)
    {
        WorkerSharedPtr w_sp(new Worker(protocol, queue, i));
        worker_v.push_back(w_sp);
        
        rc = w_sp->start();
        if (! rc){
            LOG(ERROR) << "Error:unable to create thread," << rc ;
            exit(-1);
        }
    }
    //
    // Start a Monitor thread that will check the workers are not zombies
    //
    Monitor   monitor{worker_v,  (int)nbr_workers};
    monitor.start();
    //
    // wait for the listener - if it ends kill everything
    //
    pthread_join(listener.pthread, NULL);
    
}

//Server* createServer(unsigned nbr_workers)
//{
//    return new Server(nbr_workers);
//}


