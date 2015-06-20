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
        this->prepareFile();
        /*
        
        this->writeSequential();   
        this->readSequential();
        this->writeRandom();
        this->readRandom();
         */
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
    }
    else if(blockType == Benchmark::MagMiB){
        this->magSize = Benchmark::MiB;
    }
    else if(blockType == Benchmark::MagKiB){
        this->magSize = Benchmark::KiB;
    }
    else {
        this->magSize = Benchmark::KiB;
    }
}

/**
 * Write the file total size before start the operations.
 */
void Benchmark::prepareFile(){
    
    ofstream fileWrite(this->testFilePath, ios::binary);   
    timer.start();
    
    // Write to file using bytes as index. Pass OK
    // Real results too.
    
    for(ulong magIdx = 0; magIdx < this->sizeRepeats; magIdx++){        
        for(ulong byteIdx = 0; byteIdx < this->magSize; byteIdx++){
            //write byte
            //timer.start();
            fileWrite.put('0');
            //timer.stop();
        }
    }
    
    
    // Test write with buffered stream. Pass OK
    // Real results
    /*
    ulong bufferSize = this->sizeRepeats * this->magSize;
    const char* buffer = new char[bufferSize];
    fileWrite.write(buffer, bufferSize);
    */
    timer.stop();
    cout << "Time to create a " << this->sizeRWInMiB << "MiB file: " << timer.getDuration() << " seconds" << endl;
    cout << "Throughput: " << this->sizeRWInMiB / timer << "MiB/s" << endl;
    fileWrite.close();
    
}

/**
 * Logic to write sequential and register
 */
void Benchmark::writeSequential(){
    
        
    double byByteCounter = 0;
    
    Timer MiBTimer;
    ofstream file;
    
    // open the file    
    file.open( this->testFilePath, ios::binary);
    
    timer.start();
    
    // Write to file using bytes as index
    for(ulong magIdx = 0; magIdx < this->sizeRepeats; magIdx++){
        
        for(ulong byteIdx = 0; byteIdx < this->magSize; byteIdx++){

            MiBTimer.start();
            //write byte
            file << '0';
            MiBTimer.stop();
            byByteCounter += MiBTimer.getDuration();
 
        }
        
    }
    
    timer.stop();
    file.close();
    
    // Get time of this action
    this->totalTime += byByteCounter;//timer.getDuration();
    
    cout << "Write sequential: " << byByteCounter << " seconds" << endl;        
    printf("Throughput: %f MiB/s\n", sizeRWInMiB / byByteCounter);
}

void Benchmark::readSequential(){
    
    double byByteCounter = 0;
    char b;
    Timer MiBTimer;
    ifstream file;
    
    // open the file
    file.open(this->testFilePath, ios::binary);    
    
    timer.start();
    
    // Read file less the size
    for(ulong magIdx = 0; magIdx < this->sizeRepeats; magIdx++){
        for(ulong byteIdx = 0, MiBCounter = 0; byteIdx < this->magSize; byteIdx++, MiBCounter++){
            MiBTimer.start();
            // Read byte
            file >> b;
            MiBTimer.stop();
            byByteCounter += MiBTimer;
        }        
    }
    
    timer.stop();
    
    // Get time of this action
    this->totalTime += timer.getDuration();
    
    file.close();
    
    cout << "Read sequential: " << timer << " seconds" << endl;
    //cout << "Medium time: " << byByteCounter / Benchmark::MiB << " MiB/s" << endl;
    
    printf("Throughput: %f MiB/s\n", sizeRWInMiB / timer);
}


void Benchmark::writeRandom(){
    srand( time(NULL));
    ulong seek_tmp = 0;
    double byByteCounter = 0;
    char b = 0;
    Timer MiBTimer;
    ofstream file;
    
    // open the file    
    file.open(this->testFilePath, ios::binary);
        
    timer.start();
    
    // Write to file using bytes as index
    for(ulong magIdx = 0; magIdx < this->sizeRepeats; magIdx++){
        
        for(ulong byteIdx = 0; byteIdx < this->magSize; byteIdx++){

            MiBTimer.start();
            //write byte
            seek_tmp = (rand() % this->sizeRepeats) % this->magSize;
            file.seekp(seek_tmp);
            file << b;
            
            MiBTimer.stop();
            byByteCounter += MiBTimer;
 
        }
        
    }
    
    timer.stop();
    file.close();
    
    // Get time of this action
    this->totalTime += timer.getDuration();
    
    cout << "Write random: " << timer << " seconds" << endl;        
    printf("Throughput: %f MiB/s\n", sizeRWInMiB / timer);    
}

void Benchmark::readRandom(){
    
}