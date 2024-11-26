#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#define BUF_SIZE 1024
#define MAX_CONNS 1024
#define QUEUE_LIMIT 5

int max(int a, int b){
    if(a > b) return a;
    return b;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s <listening_port>", argv[0]);
        return -1;
    }
    int sockfd, newfd, addrlen, ready_cnt, max_fd = 0;
    int port = atoi(argv[1]);
    int clientfds[MAX_CONNS];
    for(int i=0; i<MAX_CONNS; i++){
        clientfds[i] = 0;
    }
    fd_set rfds;
    struct timeval tv;
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
    if(listen(sockfd, QUEUE_LIMIT) < 0){
        printf("Can't listen\n");
        return -1;
    }
    while(1){
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
        for(int i=0; i<MAX_CONNS+1; i++){
            if(clientfds[i]==1){
                // printf("FDSET 1 to %d\n", i);
                FD_SET(i, &rfds);
            }
        }
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        ready_cnt = select(MAX_CONNS+1, &rfds, NULL, NULL, &tv);
        // printf("%d\n", ready_cnt);
        for(int i=0; i<MAX_CONNS+1; i++){
            if(clientfds[i] && FD_ISSET(i, &rfds)){
                char buffer[BUF_SIZE];
                read(i, buffer, BUF_SIZE-1);
                write(i, "World", 6);
                close(i);
                // printf("Setting 0 to: %d\n", i);
                clientfds[i] = 0;
                FD_CLR(i, &rfds);
            }
        }
        if(FD_ISSET(sockfd, &rfds)){
            if((newfd = accept(sockfd, (struct sockaddr*)&addr, &addrlen)) < 0){
                printf("Couldn't establish connection\n");
                return -1;
            }
            // printf("Setting 1 to: %d\n", newfd);
            clientfds[newfd] = 1;
            max_fd = max(max_fd, newfd);
        }
    }
    close(sockfd);
    return 0;
}