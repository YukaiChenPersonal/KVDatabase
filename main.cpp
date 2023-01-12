/*
Author:     Yukai Chen
Description:   Test and demonstration of supported functions of this program.
*/

#include <iostream>
#include "skiplist.h"

#define OUTPUT_FLIE "./store/DumpFile"

int main(){
    SkipList<int, std::string> mySkipList(6);
    mySkipList.insertByKey(1, "This ");
    mySkipList.insertByKey(2, "is ");
    mySkipList.insertByKey(5, "a ");
    mySkipList.insertByKey(7, "function ");
    mySkipList.insertByKey(10, "test.");
    
    std::cout << "The size of this data base is " << mySkipList.getSize()<< std::endl;
    mySkipList.display();

    mySkipList.writeToDisk();

    mySkipList.searchByKey(2);
    mySkipList.searchByKey(6);

    mySkipList.deleteByKey(7);

    std::cout << "The size of this data base is " << mySkipList.getSize()<< std::endl;
    mySkipList.display();

}