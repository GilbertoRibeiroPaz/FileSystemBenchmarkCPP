/* 
 * File:   main.cpp
 * Author: gilberto
 *
 * Created on June 1, 2015, 8:39 PM
 */


#include <iostream>
#include "Benchmark.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    Benchmark bnk;
    bnk.setEnv(".", 1, Benchmark::MiB);
    bnk.run();
    
    
    return 0;
}

