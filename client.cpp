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

#define SERV_PORT 9527
#define BUFFSIZE 4096
#define COMMAND_LENGTH 200


void sys_err(const char *str){
    perror(str);
    exit(1);
}

int main(int argc, char* argv[]){
    int cfd = 0;
    char buf[BUFFSIZE], command[COMMAND_LENGTH];
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1){
        sys_err("socket error");
    }

    struct sockaddr_in  serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);


    int ret = connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret != 0){
        sys_err("connect err");
    }

    while(1){
        std::cout << "Please enter command..." << std::endl;
        std::cin >> command;
        write(cfd, command, sizeof(command));
        int relen = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, relen);
    }

    close(cfd);
    return 0;

}