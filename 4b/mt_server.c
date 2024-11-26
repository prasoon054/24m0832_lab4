#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#define BUF_SIZE 1024
#define MAX_CONNS 1024

typedef struct client_info{
    int fd;
    struct sockaddr_in addr;
    struct client_info* next;
} client_info;

typedef struct queue{
    client_info* front;
    client_info* rear;
    int size;
} queue;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

queue* create_queue(){
    queue* q = (queue*)malloc(sizeof(queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

void enqueue(queue* q, client_info* client){
    pthread_mutex_lock(&lock);
    if(q->rear==NULL){
        q->front = q->rear = client;
    }
    else{
        q->rear->next = client;
        q->rear = client;
    }
    q->size++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
}

client_info* deque(queue* q){
    pthread_mutex_lock(&lock);
    while(q->size==0){
        pthread_cond_wait(&cond, &lock);
    }
    client_info* client = q->front;
    q->front = q->front->next;
    if(q->front==NULL){
        q->rear = NULL;
    }
    q->size--;
    pthread_mutex_unlock(&lock);
    return client;
}

void* handle_client(void* arg){
    queue *q = (queue*)arg;
    while(1){
        client_info* client = deque(q);
        char buff[BUF_SIZE];
        int sz = read(client->fd, buff, BUF_SIZE-1);
        buff[sz] = '\0';
        if(strcmp("hello", buff)==0){
            write(client->fd, "world", 6);
        }
        close(client->fd);
        free(client);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage: %s <listening_port> <thread_pool_size>\n", argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);
    int num_th = atoi(argv[2]);
    int sockfd, newfd, addrlen;
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
    queue* q = create_queue();
    pthread_t th[num_th];
    for(int i=0; i<num_th; i++){
        if(pthread_create(&th[i], NULL, handle_client, (void*)q)!=0){
            close(sockfd);
            return -1;
        }
        pthread_detach(th[i]);
    }
    while(1){
        client_info* client = (client_info*)malloc(sizeof(client_info));
        if(!client){
            printf("Memory allocation failed\n");
            continue;
        }
        if((newfd = accept(sockfd, (struct sockaddr*)&addr, &addrlen)) < 0){
            printf("Couldn't establish connection\n");
            free(client);
            return -1;
        }
        client->fd = newfd;
        client->next = NULL;
        enqueue(q, client);
    }
    close(sockfd);
    return 0;
}
