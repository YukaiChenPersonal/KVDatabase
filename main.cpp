/*
Author:     Yukai Chen
Description:   Test and demonstration of supported functions of this program.
*/

#include <iostream>
#include <fstream>
#include <cerrno>
#include <pthread.h>
#include <sys/socket.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>
#include "skiplist.h"
#include "ParseInput.h"

#define SERV_PORT 9527
#define BUFFSIZE 4096

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    std::cout << "Begin Database Operation..." << std::endl;
    SkipList<int, std::string> myskiplist(6);
    std::ifstream fin(FILE_DIR);
    bool isDatabase = (fin.good()) ? true : false;

    if (isDatabase)
    {
        std::cout << "Database already exists..." << std::endl;
        myskiplist.readFromDisk();
    }
    else
    {
        std::cout << "Database dosen't exist..." << std::endl;
    }

    int lfd = 0, cdf = 0;
    char buf[BUFFSIZE], retbuf[BUFFSIZE];
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        sys_err("socket error");
    }

    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len = sizeof(clit_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int checkBind = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (checkBind == -1)
    {
        sys_err("bind error");
    }

    listen(lfd, 128);

    int cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
    if (cfd == -1)
    {
        sys_err("accpet error");
    }

    while (1)
    {
        int ret = 0;
        ret = read(cfd, buf, sizeof(buf));

        std::vector<std::string> command = parseInput(buf);
        if (command.size() < 1){
            std::string temp = "Command error, please enter again...";
            strcpy(buf, temp.c_str());
        } else {
            std::string commandType = command[0];
            if (commandType == "insertByKey"){
                if (command.size() < 3){
                    std::string temp = "Command error, please enter again...";
                    strcpy(buf, temp.c_str());
                } else {
                    int key = stoi(command[1]);
                    std::string value = command[2];
                    int dbRet = myskiplist.insertByKey(key, value);
                    std::cout << dbRet << std::endl;
                    if (dbRet == 0){
                        std::string temp = "Insertion successed, with key: " + command[1] + " and value: " + command[2];
                        strcpy(retbuf, temp.c_str());
                    } else{
                        std::string temp = "Insertion failed, key " + command[1] + " already exists";
                        strcpy(retbuf, temp.c_str());
                    }
                }               
            } else if (commandType == "deleteByKey") {
                if (command.size() < 2){
                    std::string temp = "Command error, please enter again...";
                    strcpy(retbuf, temp.c_str());
                } else {
                     int key = stoi(command[1]);
                    int dbRet = myskiplist.deleteByKey(key);

                    std::string temp;
                    if (dbRet == 0){
                        temp = "Delete successed.";
                    } else {
                        temp = "Delete failed, key not found!";
                    }
                    strcpy(retbuf, temp.c_str());
                }
               
            } else if (commandType == "searchByKey") {
                if (command.size() < 2) {
                    std::string temp = "Command error, please enter again...";
                    strcpy(buf, temp.c_str());
                } else {
                    int key = stoi(command[1]);
                    std::string *res = new std::string();
                    int dbRet = myskiplist.searchByKey(key, res);

                    std::string temp;
                    if (dbRet == 1) {
                        temp = "Key: " + command[1] + " found, with value: " + *res;
                    } else {
                        temp = "Key: " + command[1] + " not found!";
                    }
                    strcpy(retbuf, temp.c_str());
                }

                
            } else if (commandType == "changeByKey") {
                if (command.size() < 3) {
                    std::string temp = "Command error, please enter again...";
                    strcpy(buf, temp.c_str());
                } else {
                    int key = stoi(command[1]);
                    std::string value = command[2];
                    int dbRet = myskiplist.changeByKey(key, value);

                    std::string temp;
                    if (dbRet == 0) {
                        temp = "Change successed, with key: " + command[1] + " and new value: " + command[2];
                    } else {
                        temp = "Change failed, key: " + command[1] + " not found!";
                    }

                    strcpy(retbuf, temp.c_str());
                }

                
            } else if (commandType == "writeToDisk") {
                myskiplist.writeToDisk();
                std::string temp = "Write successful!";
                strcpy(retbuf, temp.c_str());
            } else if (commandType == "getSize"){
                int size = myskiplist.getSize();
                std::string temp = "The size of the database is: " + size;
                strcpy(retbuf, temp.c_str());
            } else {
                std::string temp = "Command error, please enter again...";
                strcpy(retbuf, temp.c_str());
            }
        }
        write(cfd, retbuf, ret);
    }

    close(lfd);
    close(cfd);
    return 0;

}