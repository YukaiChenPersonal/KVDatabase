#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

#include <string>
#include <vector>
#include "skiplist.h"


std::vector<std::string> getCommand(const std::string& str){
    std::vector<std::string> res;
    std::string temp;
    int pos = 0;
    for(int i=0; i<str.size(); i++){
        if(str[i] != ':'){
            temp += str[i];
        } else {
            res.push_back(temp);
            temp = "";
        }
    }
    return res;
}

#endif