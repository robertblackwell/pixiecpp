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
#include <unistd.h>


#include "Logger.h"
#include "Monitor.h"
#include "Worker.h"


Monitor::Monitor( WorkerVector& _workers, int _count) : workers(_workers)
{
    workers = _workers;
    count = _count;
}
Monitor::~Monitor(){
    
}
void Monitor::scanWorkerTable()
{
    int kount = 0;
    for(int i = 0; i < count ; i++)
    {
        WorkerSharedPtr w = workers[i];
        if( w->active ){
            std::cout << "Monitor active : " << i << " socket: " << workers[i]->active_socket << std::endl;
            kount ++;
        }
    }
    std::cout << "Monitor " << kount << " active " << std::endl;
}
void Monitor::main()
{
    while(true)
    {
        scanWorkerTable();
        sleep(10);
    }
}

