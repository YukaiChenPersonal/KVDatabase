#include <string>
#include <vector>
#include <iostream>
#include "skiplist.h"

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

    SkipList<int, std::string> newSkipList(6);
    newSkipList.readFromDisk();
    newSkipList.display();
    mySkipList.writeToDisk();

    std::string* temp;
    mySkipList.searchByKey(2, temp);
    mySkipList.searchByKey(6, temp);

    mySkipList.deleteByKey(7);

    std::cout << "The size of this data base is " << mySkipList.getSize()<< std::endl;
    mySkipList.display();

    mySkipList.changeByKey(5, "A ");
    mySkipList.changeByKey(7, "Function");
    mySkipList.display();

}
