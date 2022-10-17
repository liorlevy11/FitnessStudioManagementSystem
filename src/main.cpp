//
// Created by ellabeeri on 10/11/2021.
//
#include "Studio.h"
#include <iostream>
#include "fstream"


using namespace std;

Studio* backup = nullptr;


int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }

    std::ifstream file(configurationFile);
    studio.start();
    return 0;
}

