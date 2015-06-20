/* 
 * File:   Benchmark.cpp
 * Author: gilberto
 * 
 * Created on June 1, 2015, 8:39 PM
 */

#include "Benchmark.h"
#include "Timer.h"

using namespace std;


static Benchmark* instance = NULL;
/**
 * Default constructor
 */
Benchmark::Benchmark(){
    
    this->mountPoint = "";
    this->repeats = 0;
    this->sizeRepeats = 0;
    
    envIsAlreadySet = false;
    
    instance = this;
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
    
    instance = this;
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
    
    instance = this;
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
            cout << "Repeat: " << repeatIdx+1 << "/" << this->repeats << endl;
            this->reset();
            this->writeSequential();            
            this->readSequential();
            this->writeRandom();
            this->readRandom();
            
            printf("Total Time: %.10f\n", this->totalTime);            
        }
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
        
    // Write to file using bytes as index
    for(uint_fast64_t gIdx = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            
            // Start timer to each MiB measure
            timer.start();
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++){
                    file.put(0);
                }
            }
            // stop
            timer.stop();
        }        
    }    
    
    // Get time of this action
    this->totalTime += timer.totalTime();
    
    // Print out data capture
    cout << endl;
    cout << "Write sequential: " << endl;
    cout << "Exec time: " << timer.totalTime() << " seconds" << endl;       
    cout << "Throughput: " << sizeRWInMiB / timer.totalTime() << " MiB/s" << endl;
    cout << endl;
    
    // close file 
    file.close();
    
    // clear cronometer
    timer.clear();
}

void Benchmark::readSequential(){
    
    // file handler
    ifstream file;
        
    // open the file    
    file.open( this->testFilePath, ios::binary);
    
    // Read to file using bytes as index
    for(uint_fast64_t gIdx = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            
            // Start measure for each MiB
            timer.start();
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++){
                    file.get();
                }
            }
            
            // Stop measure
            timer.stop();    
        }        
    }
    

    
    // Get time of this action
    this->totalTime += timer.totalTime();
    
    // Print out date
    cout << "Read sequential: " << endl;
    cout << "Exec time: " << timer.totalTime() << " seconds" << endl;       
    cout << "Throughput: " << sizeRWInMiB / timer.totalTime() << " MiB/s" << endl;
    cout << endl;
    
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
    
    uint_fast64_t seek_tmp = 1;
    
    // open the file    
    file.open(this->testFilePath, ios::binary);
    
    // Get the byte total size
    uint_fast64_t totalSize = this->sizeRepeats * this->magSize;
    
    // Write to file using bytes as index with file seeks
    // for each KiB
    
    // TODO: Get the pointer to the of file position. NOTE: it's faster than using <file handler>.tellg(), because of a needless of jump and copy/return new allocated memory
    //file.register_callback(lastIndexEventForFileWriter, totalSize);
    
    
    for(uint_fast64_t gIdx = 0, maxFpos = 0; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            
             // Start measure for each MiB
            timer.start();
            
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                
                // random seek position for each KiB
                seek_tmp = (rand() % totalSize);
                file.seekp(seek_tmp, ios::beg);
                maxFpos = seek_tmp;
                
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++, maxFpos++){    
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
    
    // Print results
    cout << "Write random: " << endl;
    cout << "Exec time: " << timer.totalTime() << " seconds" << endl;        
    cout << "Throughput: " << sizeRWInMiB / timer.totalTime() << " MiB/s" << endl;    
    cout << endl;
    
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
    ulong seek_tmp;
    
    // open the file    
    file.open(this->testFilePath, ios::binary);
    
    // Get the byte total size
    uint_fast64_t totalSize = this->sizeRepeats * this->magSize;
        
    // Read from file using bytes as index with file seeks
    for(uint_fast64_t gIdx = 0, maxFpos; gIdx < gibs; gIdx++){
        for(uint_fast64_t mIdx = 0; mIdx < mibs; mIdx++){
            
            // Start measure for each MiB
            timer.start();
            
            for(uint_fast64_t kIdx = 0; kIdx < kibs; kIdx++){
                
                // random seek position for each KiB
                seek_tmp = (rand() % totalSize);
                file.seekg(seek_tmp, ios::beg);
                maxFpos = seek_tmp;
                
                for(uint_fast64_t bIdx = 0; bIdx < 1024; bIdx++, maxFpos++){    
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
    
    // Print results
    cout << "Read random: " << endl; 
    cout << "Exec time: " << timer.totalTime() << " seconds" << endl;        
    cout << "Throughput: " << sizeRWInMiB / timer.totalTime() << " MiB/s" << endl;    
    cout << endl;
    
    // close file
    file.close();
    
    // clear cronometer
    timer.clear();
}
