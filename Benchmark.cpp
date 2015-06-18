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
    if(this->envIsAlreadySet){
        sizeRWInMiB = (this->sizeRepeats * this->magSize) / Benchmark::MiB;
        this->writeSequential();
        this->readSequential();
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
 * Logic to write sequential and register
 */
void Benchmark::writeSequential(){
    
    // open the file
    const char* filePath = std::string(this->mountPoint + "/sequential").c_str();
    double byByteCounter = 0;
    
    Timer MiBTimer;    
    // c++ ofstream is slow. Why?
    //std::ofstream file(filePath, std::ios::binary);
    FILE* file = fopen(filePath, "wb");
    //int fd = open(filePath,)
    char* bfr = new char[1];
    
    timer.start();
    
    // Write to file using bytes as index
    for(ulong magIdx = 0; magIdx < this->sizeRepeats; magIdx++){
        
        for(ulong byteIdx = 0; byteIdx < this->magSize; byteIdx++){

            MiBTimer.start();
            //write byte
            //file.write(bfr, 1);
            fwrite(bfr, 1, 1, file);
            MiBTimer.stop();
            byByteCounter += MiBTimer;
 
        }
        
    }
    
    timer.stop();
    //file.close();
    fclose(file);
    // Get time of this action
    this->totalTime += timer.getDuration();
    
    
    
    cout << "Write duration: " << timer << " seconds" << endl;        
    printf("Throughput: %f MiB/s\n", sizeRWInMiB / timer.getDuration());
}

void Benchmark::readSequential(){
    // open the file
    const char* filePath = std::string(this->mountPoint + "/sequential").c_str();
    double byByteCounter = 0;
    
    Timer MiBTimer;
    
    std::fstream file(filePath, std::ios::binary);
    char* bfr = new char[1];
    
    timer.start();
    
    
    // Read file less the size
    for(ulong magIdx = 0; magIdx < this->sizeRepeats; magIdx++){
        for(ulong byteIdx = 0, MiBCounter = 0; byteIdx < this->magSize; byteIdx++, MiBCounter++){
            MiBTimer.start();
            // Read byte
            file.read(bfr, 1);
            MiBTimer.stop();
            byByteCounter += MiBTimer;
        }        
    }
    

    
    timer.stop();
    
    // Get time of this action
    this->totalTime += timer.getDuration();
    
    file.close();
    
    cout << "Read duration: " << timer << " seconds" << endl;
    //cout << "Medium time: " << byByteCounter / Benchmark::MiB << " MiB/s" << endl;
    
    printf("Medium Time: %.15f MiB/s\n", sizeRWInMiB / byByteCounter);
}