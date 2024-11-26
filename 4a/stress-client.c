#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <server_ip> <server_port> <client_id>\n", argv[0]);
        return -1;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024] = "hello";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Client %s: Connection failed\n", argv[3]);
        return -1;
    }
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, 1024, 0);
    printf("Client %s received: %s\n", argv[3], buffer);
    close(sockfd);
    return 0;
}
