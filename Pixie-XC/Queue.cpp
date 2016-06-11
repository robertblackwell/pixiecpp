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


#include "Logger.h"


#include "Queue.h"

Queue::Queue()
{
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_cv, NULL);
}
    
Queue::~Queue()
{
    pthread_cond_destroy(&count_cv);
    pthread_mutex_destroy(&count_mutex);
    LOG(INFO) << "Queue - dealloc";
    
}
    
int Queue::remove()
{
    pthread_mutex_lock(&count_mutex);
    while( q.size() == 0 ){
        pthread_cond_wait(&count_cv, &count_mutex);
    }
    int r = q[0];
    q.pop_front();
    pthread_mutex_unlock(&count_mutex);
    return r;
}
    
void Queue::add(int a)
{
    pthread_mutex_lock(&count_mutex);
    q.push_back(a);
    if( q.size() == 1 ){
        pthread_cond_broadcast(&count_cv);
    }
    pthread_mutex_unlock(&count_mutex);
}

