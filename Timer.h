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
    
    friend std::ostream& operator<<(std::ostream& os, Timer& t){
        os << t.lastDuration;
        return os;
    }
    
    friend double operator/(Timer& t1, double d){
        if(d == 0) return 0;
        return t1.lastDuration / d;
    }
    
    friend double operator+=(double& d, Timer& t){
        d += t.lastDuration;
        return d;
    }
};

#endif	/* TIMER_H */