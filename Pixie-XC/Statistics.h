#pragma once


class ElapsedTime
{
public:
    struct timeval      _startTime;
    long                absolute_start_time;
    struct timeval      _stopTime;
    long                absolute_stop_time;
    long _elapsedTime;
    void start();
    long stop();
};
class Statistics
{
    int messageCount;
    int characterCount;
    ElapsedTime    currentClock;
    std::vector<long>  times;
    
    Statistics();    
    void addMessage();
    void addCharacterCount(int more);
    void startClock();
    void stopClock();
    void print();
};
