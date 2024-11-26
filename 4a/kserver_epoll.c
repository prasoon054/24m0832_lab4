#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#define BUF_SIZE 1024
#define MAX_CONNS 1024
#define QUEUE_LIMIT 5

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s <listening_port>\n", argv[0]);
        return -1;
    }
    int sockfd, newfd, addrlen, epollfd, nfds;
    int port = atoi(argv[1]);
    struct sockaddr_in addr;
    struct epoll_event event, events[MAX_CONNS];
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
    epollfd = epoll_create1(0);
    if(epollfd==-1){
        printf("Failed to create epoll file descriptor\n");
        return -1;
    }
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event)==-1){
        printf("Failed to add socket to epoll\n");
        return -1;
    }
    while(1){
        nfds = epoll_wait(epollfd, events, MAX_CONNS, -1);
        if(nfds==-1){
            printf("Couldn't do poll wait\n");
            return -1;
        }
        for(int i=0; i<nfds; i++){
            if(events[i].data.fd==sockfd){
                newfd = accept(sockfd, (struct sockaddr*)&addr, &addrlen);
                if(newfd < 0){
                    printf("Couldn't establish connection\n");
                    return -1;
                }
                event.events = EPOLLIN;
                event.data.fd = newfd;
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &event)==-1){
                    printf("Failed to add fd to epoll\n");
                    close(newfd);
                }
            }
            else{
                char buffer[BUF_SIZE];
                read(events[i].data.fd, buffer, sizeof(buffer));
                write(events[i].data.fd, "World", 6);
                close(events[i].data.fd);
                epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &event);
            }
        }
    }
    close(sockfd);
    return 0;
}
