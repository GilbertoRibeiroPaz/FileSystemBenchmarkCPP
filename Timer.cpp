#include "Timer.h"

Timer::Timer(){
    this->lastDuration = 0;
    this->acumulator = 0;
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
    this->acumulator += this->lastDuration;
}

double Timer::getDuration(){
    return this->lastDuration;
}

void Timer::clear(){
    this->acumulator = 0;
}

double Timer::totalTime(){
    return this->acumulator;
}