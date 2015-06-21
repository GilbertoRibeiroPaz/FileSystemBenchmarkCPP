#include "Timer.h"

Timer::Timer(){
    this->lastDuration = 0;
    this->acumulator = 0;
    this->size = 1;
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
    
    this->indResult.push_back(duration);
    this->lastDuration = duration;
    this->acumulator += this->lastDuration;
}

double Timer::getDuration(){
    return this->lastDuration;
}

void Timer::clear(){
    this->acumulator = 0;
    this->lastDuration = 0;
    this->indResult.clear();
    this->size = 1;
}

double Timer::totalTime(){
    return this->acumulator;
}

void Timer::setSetSize(ulong size){
    this->indResult.resize(size);
    this->size = size;
}

double Timer::averageTime(){
    return this->acumulator / size;
}

double Timer::variance(){
    double medium = this->averageTime();
    double sumSamples = 0;
    
    for(ulong aIdx = 0; aIdx < size; aIdx++){
        double dev = (indResult[aIdx] - medium);
        sumSamples += (dev * dev);
    }
    
    return sumSamples / size;    
}
double Timer::defaultDeviation(){
    return sqrt(this->variance());
}