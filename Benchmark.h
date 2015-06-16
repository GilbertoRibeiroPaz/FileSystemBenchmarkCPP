/* 
 * File:   Benchmark.h
 * Author: gilberto
 *
 * Created on June 1, 2015, 8:39 PM
 */

#ifndef BENCHMARK_H
#define	BENCHMARK_H

#include <iostream>
#include <fstream>
#include <string>
#include "Timer.h"


class Benchmark {
public:
    Benchmark();
    Benchmark(std::string mountPoint, ulong repeats, ulong size);
    Benchmark(const Benchmark& orig);
    virtual ~Benchmark();
    void setEnv(std::string mountPoint, ulong repeats, ulong size);
    void run();
    
    static const ulong MiB = 1024*1024;
    
private:
    std::string mountPoint;
    ulong size;
    ulong repeats;
    ulong readSequentialThroughput;
    ulong writeSequentialThroughput;
    ulong readRandomThroughput;
    ulong writeRandomThroughtput;
    
    double totalTime;
    
    Timer timer;
    
    void reset();
    
    void writeSequential();
    void readSequential();
    
    
    bool envIsAlreadySet;
    
    
};

#endif	/* BENCHMARK_H */

