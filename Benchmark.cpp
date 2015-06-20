/* 
 * File:   Benchmark.cpp
 * Author: gilberto
 * 
 * Created on June 1, 2015, 8:39 PM
 */

#include "Benchmark.h"
#include "Timer.h"

using namespace std;

/**
 * Default constructor
 */
Benchmark::Benchmark(){
    
    this->mountPoint = "";
    this->repeats = 0;
    this->sizeRepeats = 0;
    
    envIsAlreadySet = false;
}

/**
 * Constructor setting the environment
 * @param mountPoint
 * @param repeats
 * @param size
 */
Benchmark::Benchmark(std::string mountPoint, ulong repeats, ulong size, BlockMagType type) {
    
    this->mountPoint = mountPoint;
    this->repeats = repeats;
    this->sizeRepeats = size;
    this->blockType = type;
    
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
    this->sizeRepeats = orig.sizeRepeats;    
    this->blockType = orig.blockType;
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
void Benchmark::setEnv(std::string mountPoint, ulong repeats, ulong size, BlockMagType type){
    this->reset();
    
    this->mountPoint = mountPoint;
    this->repeats = repeats;
    this->sizeRepeats = size;    
    this->blockType = type;
    
    envIsAlreadySet = true;
}

/**
 * Run the benchmark only if the environment
 * is already set.
 */
void Benchmark::run(){
    
    this->reset();
    this->setMagTestSize();
    this->setTestFilePath();
    
    if(this->envIsAlreadySet){
        sizeRWInMiB = (this->sizeRepeats * this->magSize) / Benchmark::MiB;
               
        this->writeSequential();   
        this->readSequential();
        this->writeRandom();
        this->readRandom();
        
        printf("Total Time: %.10f\n", this->totalTime);
    }
}

/**
 * Get results produced by benchmark
 * @return result
 */
std::string Benchmark::getResults(){
    return this->results;
}

/**
 * Set throughput and total time to zero.
 */
void Benchmark::reset(){
    results = "";
    readRandomThroughput = readSequentialThroughput = 0;
    writeRandomThroughtput = writeSequentialThroughput = 0;
    totalTime = 0;
    
}

/**
 * Set the the file path to write the test.
 */
void Benchmark::setTestFilePath(){
    this->testFilePath = this->mountPoint + "/testfile";
}

/**
 * Get selected magnitude and set max size.
 */
void Benchmark::setMagTestSize(){
    if(blockType == Benchmark::MagGiB){
        this->magSize = Benchmark::GiB;   
        this->gibs = this->sizeRepeats;
        this->mibs = 1024;
        this->kibs = 1024;
    }
    else if(blockType == Benchmark::MagMiB){
        this->magSize = Benchmark::MiB;
        this->gibs = 1;
        this->mibs = this->sizeRepeats;
        this->kibs = 1024;
    }
    else if(blockType == Benchmark::MagKiB){
        this->magSize = Benchmark::KiB;
        this->gibs = 1;
        this->mibs = 1;
        this->kibs = this->sizeRepeats;               
    }
    else {
        this->magSize = Benchmark::KiB;
        this->gibs = 1;
        this->mibs = 1;
        this->kibs = this->sizeRepeats;
    }
}


/**
 * Logic to write sequential and register
 */
void Benchmark::writeSequential(){
        
    // file handler
    ofstream file;
    
    // open the file    
    file.open( this->testFilePath, ios::binary);
    
    // Start measure
    timer.start();
    
    // Test write with timer by MiB
    
    // Write to file using bytes as index
    for(uint_fast64_t gIdx = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++){
                    file.put(bIdx);
                }
            }
        }        
    }
    
    // Stop measure
    timer.stop();    
    
    // Get time of this action
    this->totalTime += timer;
    
    // Print out date
    cout << "Write sequential: " << timer << " seconds" << endl;       
    cout << "Throughput: " << sizeRWInMiB / timer << " MiB/s" << endl;
    
    // close file 
    file.close();
}

void Benchmark::readSequential(){
    
    // file handler
    ifstream file;
    
    // open the file    
    file.open( this->testFilePath, ios::binary);
    
    // Start measure
    timer.start();
    
    // Read to file using bytes as index
    for(uint_fast64_t gIdx = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++){
                    file.get();
                }
            }
        }        
    }
    
    // Stop measure
    timer.stop();    
    
    // Get time of this action
    this->totalTime += timer;
    
    // Print out date
    cout << "Read sequential: " << timer << " seconds" << endl;       
    cout << "Throughput: " << sizeRWInMiB / timer << " MiB/s" << endl;
    
    // close file 
    file.close();
}


void Benchmark::writeRandom(){
    
    // Start 
    srand( time(NULL));
    
    // File handler
    ofstream file;
    ulong seek_tmp;
    
    // open the file    
    file.open(this->testFilePath, ios::binary);
    
    // Start measure
    timer.start();
    
    // Get the byte total size
    uint_fast64_t totalSize = this->sizeRepeats * this->magSize;
    
    // Write to file using bytes as index with file seeks
    // for each KiB
    for(uint_fast64_t gIdx = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                
                // random seek position for each KiB
                seek_tmp = (rand() % totalSize);
                file.seekp(seek_tmp);
                
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++){
                    file.put(bIdx);
                }
                
            }
        }        
    }
    
    // Stop measure
    timer.stop();   
    
    // Get time of this action
    this->totalTime += timer;
    
    // Print results
    cout << "Write random: " << timer << " seconds" << endl;        
    cout << "Throughput: " << sizeRWInMiB / timer << " MiB/s" << endl;    
    
    // close file
    file.close();
}

void Benchmark::readRandom(){
        
    // Start 
    srand( time(NULL));
    
    // File handler
    ifstream file;
    ulong seek_tmp;
    
    // open the file    
    file.open(this->testFilePath, ios::binary);
    
    // Get the byte total size
    uint_fast64_t totalSize = this->sizeRepeats * this->magSize;
    
    // Start measure
    timer.start();
    
    // Read from file using bytes as index with file seeks
    for(uint_fast64_t gIdx = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                
                // random seek position for each KiB
                seek_tmp = (rand() % totalSize);
                file.seekg(seek_tmp);
                
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++){
                    // Read a byte
                    file.get();
                }                
            }
        }        
    }
    
    // Stop measure
    timer.stop();   
    
    // Get time of this action
    this->totalTime += timer;
    
    // Print results
    cout << "Read random: " << timer << " seconds" << endl;        
    cout << "Throughput: " << sizeRWInMiB / timer << " MiB/s" << endl;    
    
    // close file
    file.close();
}