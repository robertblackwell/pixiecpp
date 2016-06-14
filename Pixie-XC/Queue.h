//
//  main.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 12/27/15.
//  Copyright © 2015 Blackwellapps. All rights reserved.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <easylogging++.h>

class Queue
{
    std::deque<int>  q;
    pthread_mutex_t  count_mutex;
    pthread_cond_t   count_cv;

    public :
    
    Queue();
    ~Queue();
    int remove();
    void add(int a);
};

#endif