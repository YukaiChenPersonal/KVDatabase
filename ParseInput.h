#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstdio>
#include <pthread.h>
#include <string>
#include <cstring>
#include <vector>

std::vector<std::string> parseInput(const char arr[]){
    std::vector<std::string> res;
    std::string temp;
    int pos = 0;
    for(int i=0; i<strlen(arr); i++){
        if(arr[i] != ':'){
            temp += arr[i];
        } else {
            res.push_back(temp);
            temp = "";
        }
    }
    return res;
}


