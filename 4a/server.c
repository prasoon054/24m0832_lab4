#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUF_SIZE 1024
#define MAX_CONNS 1024

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s <listening_port>\n", argv[0]);
        return -1;
    }
    int sockfd, newfd, addrlen;
    int port = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    addrlen = sizeof(addr);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Socket creation error\n");
        return -1;
    }
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        printf("Socket bind error\n");
        return -1;
    }
    if(listen(sockfd, 0) < 0){
        printf("Can't listen\n");
        return -1;
    }
    while(1){
        char buffer[BUF_SIZE];
        if((newfd = accept(sockfd, (struct sockaddr*)&addr, &addrlen)) < 0){
            printf("Couldn't establish connection\n");
            return -1;
        }
        read(newfd, buffer, BUF_SIZE-1);
        write(newfd, "World", 6);
        close(newfd);
    }
    close(sockfd);
    return 0;
}
