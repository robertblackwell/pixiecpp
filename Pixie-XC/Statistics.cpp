#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cassert>

#include "Logger.h"
#include "Statistics.h"

void ElapsedTime::start()
{
    gettimeofday(&_startTime, NULL);
    absolute_start_time= (((_startTime.tv_sec * 1000) + (_startTime.tv_usec / 1000)));
}
long ElapsedTime::stop()
{
    gettimeofday(&_stopTime, NULL);
    absolute_stop_time= (((_stopTime.tv_sec * 1000) + (_stopTime.tv_usec / 1000)));
    _elapsedTime =  absolute_stop_time - absolute_start_time;
    
    return _elapsedTime;
}

Statistics::Statistics()
{
    messageCount    = 0;
    characterCount  = 0;
}
    
void Statistics::addMessage()
{
    messageCount++;
}
    
void Statistics::addCharacterCount(int more)
{
    characterCount += more;
}
    
void Statistics::startClock()
{
    currentClock.start();
}
    
void Statistics::stopClock()
{
    long t = currentClock.stop();
    times.push_back(t);
}
void Statistics::print()
{
    
}

