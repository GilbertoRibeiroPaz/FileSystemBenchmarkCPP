#include "Timer.h"

static double acumulator = 0;

Timer::Timer(){
    lastDuration = 0;
    acumulator = 0;
}

void Timer::start(){
    gettimeofday(&startTime, NULL);
}


void Timer::stop(){
    
    timeval endTime;
    long seconds, useconds;
    double duration;

    gettimeofday(&endTime, NULL);

    seconds  = endTime.tv_sec  - startTime.tv_sec;
    useconds = endTime.tv_usec - startTime.tv_usec;

    duration = seconds + useconds/1000000.0;
    
    this->lastDuration = duration;
    acumulator += this->lastDuration;
}

double Timer::getDuration(){
    return this->lastDuration;
}

void Timer::clear(){
    acumulator = 0;
}

double Timer::getAcumulatedTime(){
    return acumulator;
}