/* 
 * File:   ParametersParser.h
 * Author: gilberto
 *
 * Created on June 16, 2015, 9:06 PM
 */

#ifndef PARAMETERSPARSER_H
#define	PARAMETERSPARSER_H

#include "Benchmark.h"
#include <iostream>
#include <string.h>

class ParametersParser{
    std::string mountPoint;
    int repeats;
    int timesMag;
    Benchmark::BlockMagType magType;
    
public:
    
    ParametersParser(int argc, char** argv){
        this->parse(argc, argv);
    }
    
    /**
     * Parse the arguments passed
     * @param argc
     * @param argv
     */
    void parse(int argc, char** argv){
        
        if(argc < 3){
            std::string usage = "";
            usage = "<mount point> <repeats> <(timesmagnitude)magnitude>";            
            std::cout << "Usage: " << argv[0] << usage << std::endl;
            return;
        }
        
        this->mountPoint = (const char*)argv[1];
        this->repeats = atoi( argv[2]);
        
        // Defaults
        this->magType = Benchmark::MagKiB;
        this->timesMag = 0;
        
        // parse magnitude
        char* lastParamenter = argv[3];
        int lastParamenterSize = strlen(lastParamenter);

        for(int idx = 0; idx < lastParamenterSize; idx++) {
            
            char c = lastParamenter[idx];
            
            if ( (c == 'k' || c == 'K') && idx != 0) {
                this->magType = Benchmark::MagKiB;
            }
            else if ( (c == 'm' || c == 'M') && idx != 0) {
                this->magType = Benchmark::MagMiB;
            }
            else if ( (c == 'g' || c == 'G') && idx != 0) {
                this->magType = Benchmark::MagGiB;
            }
            else if ( isdigit(c)) {
                this->timesMag *= 10;
                this->timesMag += (c -'0');
            }
            else {
                cout << "Error on 3th parameter. Format accepted: (count)[k|K|m|M|g|G]" << endl;
                cout << "E.G.: " << argv[0] << " ~/ 1 1G" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    
    /**
     * Getter repeats
     * @return repeats
     */
    int getRepeats(){
        return this->repeats;
    }
    
    /**
     * Getter times the magnitude repeats
     * @return timesMag
     */
    int getTimesMag(){
        return this->timesMag;
    }
    
    /**
     * Getter to the mount point
     * @return mountPoint
     */
    std::string getMountPoint(){
        return this->mountPoint;
    }
    
    /**
     * Getter to the magnitude type
     * @return magType
     */
    Benchmark::BlockMagType getMagType(){
        return this->magType;
    }
};


#endif	/* PARAMETERSPARSER_H */

