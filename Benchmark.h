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
#include <time.h>
#include <stdlib.h>
#include "Timer.h"


class Benchmark {
public:
    
    static const ulong GiB = 1024*1024*1024;
    static const ulong MiB = 1024*1024;
    static const ulong KiB = 1024;
    
    enum BlockMagType{
        MagKiB, MagMiB, MagGiB
    };
    
    Benchmark();
    Benchmark(std::string mountPoint, ulong repeats, ulong size, BlockMagType type);
    Benchmark(const Benchmark& orig);
    virtual ~Benchmark();
    void setEnv(std::string mountPoint, ulong repeats, ulong size, BlockMagType type);
    void run();
    
    std::string getResults();

    
private:
    Benchmark::BlockMagType blockType;
    std::string mountPoint;
    ulong magSize;
    ulong sizeRepeats;
    ulong repeats;
    ulong readSequentialThroughput;
    ulong writeSequentialThroughput;
    ulong readRandomThroughput;
    ulong writeRandomThroughtput;
    ulong sizeRWInMiB;
    double totalTime;
    
    Timer timer;
    
    std::string results;
    const std::string TestFileName = "/testfile";
    char* testFilePath;
    
    void reset();
    void setTestFilePath();
    void setMagTestSize();
    
    // Prepare file to operations
    void prepareFile();
    
    //Read and write
    void writeSequential();
    void readSequential();
    void writeRandom();
    void readRandom();
    
    bool envIsAlreadySet;
    
    
};

#endif	/* BENCHMARK_H */

