/* 
 * File:   Benchmark.cpp
 * Author: gilberto
 * 
 * Created on June 1, 2015, 8:39 PM
 */

#include "Benchmark.h"
#include "Timer.h"

/**
 * Default constructor
 */
Benchmark::Benchmark(){
    
    this->mountPoint = "";
    this->repeats = 0;
    this->size = 0;
    
    envIsAlreadySet = false;
}

/**
 * Constructor setting the environment
 * @param mountPoint
 * @param repeats
 * @param size
 */
Benchmark::Benchmark(std::string mountPoint, ulong repeats, ulong size) {
    
    this->mountPoint = mountPoint;
    this->repeats = repeats;
    this->size = size;
    
    envIsAlreadySet = true;
}

/**
 * Copy contructor
 * @param orig
 */
Benchmark::Benchmark(const Benchmark& orig) {   
    this->reset();
    this->mountPoint = orig.mountPoint;
    this->repeats = orig.repeats;
    this->size = orig.size;    
}

/**
 * Destructor ( no dym allocation, no deallocation)
 */
Benchmark::~Benchmark() {
    
}

/**
 * Set the environment to be able to run the benchmark.
 * Use in case of use of default constructor.
 * 
 * @param mountPoint
 * @param repeats
 * @param size
 */
void Benchmark::setEnv(std::string mountPoint, ulong repeats, ulong size){
    this->reset();
    
    this->mountPoint = mountPoint;
    this->repeats = repeats;
    this->size = size;    
    
    envIsAlreadySet = true;
}

/**
 * Run the benchmark only if the environment
 * is already set.
 */
void Benchmark::run(){
    
    this->reset();
    
    if(this->envIsAlreadySet){
        this->writeSequential();
        this->readSequential();        
    }
}

/**
 * Set throughput and total time to zero.
 */
void Benchmark::reset(){
    readRandomThroughput = readSequentialThroughput = 0;
    writeRandomThroughtput = writeSequentialThroughput = 0;
    totalTime = 0;
}

/**
 * Logic to write sequential and register
 */
void Benchmark::writeSequential(){
    // open the file
    const char* filePath = std::string(this->mountPoint + "/sequential").c_str();
    
    std::ofstream file(filePath, std::ios::binary);
    
    
    timer.start();
    // Write to file using bytes as index
    for(ulong idx = 0; idx < this->size; idx++){
        file.put(0);
    }   
    
    timer.stop();
    
    file.close();
    
    printf("Writing to %s in %f seconds the amount of %d\n", filePath, timer.getDuration(), this->size);
}

void Benchmark::readSequential(){
    // open the file
    const char* filePath = std::string(this->mountPoint + "/sequential").c_str();
    std::fstream file(filePath, std::ios::binary);
    
    double byByteCounter = 0;
    
    timer.start();
    Timer byteTimer;
    
    // Read file less the size
    for(ulong idx = 0; idx < this->size; idx++){
        byteTimer.start();
        file.get();
        byteTimer.stop();
        byteTimer.AddTo(byByteCounter);
    }
    
    timer.stop();
    
    file.close();
    
    printf("Reading from %s in %f seconds the amount of %d\n", filePath, timer.getDuration(), this->size);
    printf("%f MiB/s ", byByteCounter);
}