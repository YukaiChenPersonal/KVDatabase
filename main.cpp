/*
Author:     Yukai Chen
Description:   Test and demonstration of supported functions of this program.
*/

#include <iostream>
#include <string>
#include <cstring>
#include "skiplist.h"

#define OUTPUT_FLIE "./store/DumpFile"

int main(int argc, char* argv[]){
    // if(argc <2) {
    //     std::cout << "Please assign the max level of the db" <<std::endl;
    //     return 1;
    // }
    // int level = atoi(argv[0]);
    std::string str;
    std::cout << "Please choose the database you want to manage(Type in \"d\" to use the default database.)" << std::endl;
    std::cin >> str;

    if(str == "d"){
        str = "./store/DumpFile";
    }
    SkipList<int, std::string> db(6, str);
    
    db.readFromDisk();
    
    while(1){
        std::cout << "please enter command..." << std::endl;
        std::cin >> str;
        if(str == "QUIT"){
            std::cout << "Quit Database..." << std::endl;
            break;
        }
        db.parseInput(str);
    }
    


    return 0;

}