/* 
 * File:   Timer.h
 * Author: gilberto
 *
 * Created on June 16, 2015, 3:21 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

class Timer {
private:

    timeval startTime;
    double lastDuration;
    
public:

    Timer(){
        lastDuration = 0;
    }
    
    void start(){
        gettimeofday(&startTime, NULL);
    }

    void stop(){
        timeval endTime;
        long seconds, useconds;
        double duration;

        gettimeofday(&endTime, NULL);

        seconds  = endTime.tv_sec  - startTime.tv_sec;
        useconds = endTime.tv_usec - startTime.tv_usec;

        duration = seconds + useconds/1000000.0;

        this->lastDuration = duration;
    }

    double getDuration(){
        return this->lastDuration;
    }
    
    void AddTo(double& counter){
        counter += lastDuration;
    }
};

#endif	/* TIMER_H */