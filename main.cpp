/* 
 * File:   main.cpp
 * Author: gilberto
 *
 * Created on June 1, 2015, 8:39 PM
 */


#include <iostream>
#include <string.h>
#include "Benchmark.h"
#include "ParametersParser.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    ParametersParser pp(argc, argv);
    string mp = pp.getMountPoint();
    int repeats = pp.getRepeats();
    int timesMag = pp.getTimesMag();
    Benchmark::BlockMagType magType = pp.getMagType();
    
    
    
    Benchmark bnk;
    bnk.setEnv(mp, repeats, timesMag, magType);
    bnk.run();
    
    
    return 0;
}

