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
    
    
    this->setMagTestSize();
    this->setTestFilePath();
    
    if(this->envIsAlreadySet){
        sizeRWInMiB = (this->sizeRepeats * this->magSize) / Benchmark::MiB;
        
        for(ulong repeatIdx = 0; repeatIdx < this->repeats; repeatIdx++){
            cout << endl << "Repeat: " << repeatIdx + 1 << "/" << this->repeats << endl;
            this->reset();
            this->writeSequential();            
            this->readSequential();
            this->writeRandom();
            this->readRandom();
            this->getResults();
            printf("Total Time: %.10f seconds\n", this->totalTime);
            cout << endl;
        }
    }
}

/**
 * Get results produced by benchmark
 * @return result
 */
void Benchmark::getResults(){
    
    char title[] = "%20s\t%20s\t%20s\t%20s\t%20s\n";
    char format[] = "%20s\t%20.10f\t%20.10f\t%20.10f\t%20.10f\n";
    char sep[] = "----------------------------------------------------------------------------------------------------------------------------\n";
    printf(title, "Title", "Throughput MiB/s", "Average in seconds", "Default Deviation", "Execution time");
    printf("%s", sep);
    printf(format, "Read Sequential", throughputReadSequential, averageReadSequential, defaulDevReadSequential, execTimeReadSequential);
    printf(format, "Read Random", throughputReadRandom, averageReadRandom, defaulDevReadRandom, execTimeReadRandom);
    printf(format, "Write Sequential", throughputWriteSequential, averageWriteSequential, defaulDevWriteSequential, execTimeWriteSequential);
    printf(format, "Write Random", throughputWriteRandom, averageWriteRandom, defaulDevWriteRandom, execTimeWriteRandom);
    printf("%s", sep);
}

/**
 * Set throughput and total time to zero.
 */
void Benchmark::reset(){    
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
    file.open( this->testFilePath.c_str(), ios::binary);
    
    // Set timer set amount MiBs
    timer.setSetSize(sizeRWInMiB);
    
    // Write to file using bytes as index
    for(ulong gIdx = 0; gIdx < gibs; gIdx++){
        for(ulong mIdx = 0; mIdx < mibs; mIdx++){
            
            // Start timer to each MiB measure
            timer.start();
            for(ulong kIdx = 0; kIdx < kibs; kIdx++){
                for(ulong bIdx = 0; bIdx < 1024; bIdx++){
                    file.put(0);
                }
            }
            // stop
            timer.stop();
        }        
    }    
    
    // Get time of this action
    this->totalTime += timer.totalTime();   
       
    // Capture data
    this->averageWriteSequential = timer.averageTime();
    this->defaulDevWriteSequential = timer.defaultDeviation();
    this->throughputWriteSequential = sizeRWInMiB / timer.totalTime();
    this->execTimeWriteSequential = timer.totalTime();
    
    // close file 
    file.close();
    
    // clear cronometer
    timer.clear();
}

void Benchmark::readSequential(){
    
    // file handler
    ifstream file;
        
    // open the file    
    file.open( this->testFilePath.c_str(), ios::binary);
    
    // Set timer set amount MiBs
    timer.setSetSize(sizeRWInMiB);
    
    // Read to file using bytes as index
    for(ulong gIdx = 0; gIdx < gibs; gIdx++){
        for(ulong mIdx = 0; mIdx < mibs; mIdx++){
            
            // Start measure for each MiB
            timer.start();
            for(ulong kIdx = 0; kIdx < kibs; kIdx++){
                for(ulong bIdx = 0; bIdx < 1024; bIdx++){
                    file.get();
                }
            }
            
            // Stop measure
            timer.stop();    
        }        
    }
        
    // Get time of this action
    this->totalTime += timer.totalTime();
    
    // capture data       
    this->throughputReadSequential = sizeRWInMiB / timer.totalTime();
    this->averageReadSequential = timer.averageTime();
    this->defaulDevReadSequential = timer.defaultDeviation();
    this->execTimeReadSequential = timer.totalTime();
    
    // close file 
    file.close();
    
    // clear cronometer
    timer.clear();
}


void Benchmark::writeRandom(){
    
    // Start 
    srand( time(NULL));
    
    // File handler
    ofstream file;
    
    ulong seek_tmp = 1;
    
    // open the file    
    file.open(this->testFilePath.c_str(), ios::binary);
    
    // Get the byte total size
    ulong totalSize = this->sizeRepeats * this->magSize;
    
    // Write to file using bytes as index with file seeks
    // for each KiB
    
    // TODO: Get the pointer to the of file position. NOTE: it's faster than using <file handler>.tellg(), because of a needless of jump and copy/return new allocated memory
    //file.register_callback(lastIndexEventForFileWriter, totalSize);
    
    // Set timer set amount MiBs
    timer.setSetSize(sizeRWInMiB);
    
    for(ulong gIdx = 0, maxFpos = 0; gIdx < gibs; gIdx++){
        for(ulong mIdx = 0; mIdx < mibs; mIdx++){
            
             // Start measure for each MiB
            timer.start();
            
            for(ulong kIdx = 0; kIdx < kibs; kIdx++){
                
                // random seek position for each KiB
                seek_tmp = (rand() % totalSize);
                file.seekp(seek_tmp, ios::beg);
                maxFpos = seek_tmp;
                
                for(ulong bIdx = 0; bIdx < 1024; bIdx++, maxFpos++){    
                    // Position is write, but it is writing 1024 bytes
                    // if position is 800 byte index, then it will write
                    // 800 + 1024 bytes.
                    
                    // Go back to the first byte if seek position is
                    // wider than the file total size.
                    if(maxFpos >= totalSize){
                        maxFpos = file.tellp();
                        file.seekp(0, ios::beg);
                    }
                    file.put(0);                    
                }
                
            }
            
            // Stop measure
            timer.stop();       
        }        
    }
    

    // Get time of this action
    this->totalTime += timer.totalTime();
        
    // capture results
    this->throughputWriteRandom = sizeRWInMiB / timer.totalTime();
    this->averageWriteRandom = timer.averageTime();
    this->execTimeWriteRandom = timer.totalTime();
    this->defaulDevWriteRandom = timer.defaultDeviation();
    
    // close file
    file.close();
    
    // clear cronometer
    timer.clear();
}

void Benchmark::readRandom(){
        
    // Start 
    srand( time(NULL));
    
    // File handler
    ifstream file;
    ulong seek_tmp = 1;
    
    // open the file    
    file.open(this->testFilePath.c_str(), ios::binary);
    
    // Get the byte total size
    ulong totalSize = this->sizeRepeats * this->magSize;
        
    // Set timer set amount MiBs
    timer.setSetSize(sizeRWInMiB);
    
    // Read from file using bytes as index with file seeks
    for(ulong gIdx = 0, maxFpos; gIdx < gibs; gIdx++){
        for(ulong mIdx = 0; mIdx < mibs; mIdx++){
            
            // Start measure for each MiB
            timer.start();
            
            for(ulong kIdx = 0; kIdx < kibs; kIdx++){
                
                // random seek position for each KiB
                seek_tmp = (rand() % totalSize);
                file.seekg(seek_tmp, ios::beg);
                maxFpos = seek_tmp;
                
                for(ulong bIdx = 0; bIdx < 1024; bIdx++, maxFpos++){    
                    // Position is read, but it is read 1024 bytes
                    // if position is 800 byte index, then it will write
                    // 800 + 1024 bytes.
                    
                    // Go back to the first byte if seek position is
                    // wider than the file total size.
                    if(maxFpos >= totalSize){
                        maxFpos = file.tellg();
                        file.seekg(0, ios::beg);
                    }
                    file.get();                    
                }                
            }
            
            // Stop measure
            timer.stop();   
        }        
    }
        
    // Get time of this action
    this->totalTime += timer.totalTime();
        
    // Capture results
    this->throughputReadRandom = sizeRWInMiB / timer.totalTime();
    this->averageReadRandom = timer.averageTime();
    this->execTimeReadRandom = timer.totalTime();
    this->defaulDevReadRandom = timer.defaultDeviation();
    
    // close file
    file.close();
    
    // clear cronometer
    timer.clear();
}
