/* 
 * File:   Timer.h
 * Author: gilberto
 *
 * Created on June 16, 2015, 3:21 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "proj_types.h"

using namespace std;

class Timer {
private:

    timeval startTime;
    double lastDuration;
        
public:
    Timer();
    
    void start();
    void stop();
    double getDuration();    
    void clear();    
    double getAcumulatedTime();
        
    friend std::ostream& operator<<(std::ostream& os, Timer& t){
        os << t.lastDuration;
        return os;
    }
    
    friend double operator/(Timer& t1, double d){
        if(d == 0) return 0;
        return t1.lastDuration / d;
    }
    
    friend double operator/(ulong d, Timer& t1){
        if(t1.lastDuration == 0) return 0;
        return d / t1.lastDuration;
    }
    
    friend double operator+=(double& d, Timer& t){
        d += t.lastDuration;
        return d;
    }
};

#endif	/* TIMER_H */