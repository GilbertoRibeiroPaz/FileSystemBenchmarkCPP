/* 
 * File:   Benchmark.h
 * Author: gilberto
 *
 * Created on June 1, 2015, 8:39 PM
 */

#ifndef BENCHMARK_H
#define	BENCHMARK_H

#include "proj_types.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "Timer.h"
#include "util_format.h"

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
    ulong sizeRWInMiB;
    
    uint_fast64_t gibs;
    uint_fast64_t mibs;
    uint_fast64_t kibs;
    
    double totalTime;
    
    Timer timer;
    
    // Results
    // Write sequential
    double throughputWriteSequential;
    double execTimeWriteSequential;
    double averageWriteSequential;
    double defaulDevWriteSequential;
    
    // WriteRandom;
    double throughputWriteRandom;
    double execTimeWriteRandom;
    double averageWriteRandom;
    double defaulDevWriteRandom;

    // ReadSequential;
    double throughputReadSequential;
    double execTimeReadSequential;
    double averageReadSequential;
    double defaulDevReadSequential;

    // ReadRandom;
    double throughputReadRandom;
    double execTimeReadRandom;
    double averageReadRandom;
    double defaulDevReadRandom;
    
    
    
    std::string results;
    std::string testFilePath;
    
    void reset();
    void setTestFilePath();
    void setMagTestSize();
    
    
    //Read and write
    void writeSequential();
    void readSequential();
    void writeRandom();
    void readRandom();
    
    bool envIsAlreadySet;
    
    // random access callback to seek to 0 if file pointer go further than the file total size.
    // static inline void lastIndexEventForFileWriter(ios::event ev, ios_base& stream, int index);
};

#endif	/* BENCHMARK_H */

