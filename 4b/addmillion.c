#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "common.h"

int account_balance = 0;
// int step_balance;
pthread_mutex_t lock;

void *increment(void* arg){
    int step_balance = *(int *)arg;
    int curr_balance = 0;
    for (int i = 0; i < step_balance; i++){
        curr_balance++;
    }
    pthread_mutex_lock(&lock);
    account_balance += curr_balance;
    pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Usage ./addmillion <Number of threads>\n");
        return -1;
    }
    double start_time = GetTime();
    int threadNum = atoi(argv[1]);
    int step_balance = ((1<<11)*(int)1e6)/threadNum;
    pthread_t th[threadNum];
    int i;
    pthread_mutex_init(&lock, NULL);
    for (i = 0; i < threadNum; i++){
        if (pthread_create(th + i, NULL, &increment, &step_balance) != 0){
            perror("Failed to create thread");
            return 1;
        }
        printf("Transaction %d has started\n", i);
    }
    for (i = 0; i < threadNum; i++){
        if (pthread_join(th[i], NULL) != 0){
            return 2;
        }
        printf("Transaction %d has finished\n", i);
    }
    printf("Account Balance is : %d\n", account_balance);
    double end_time = GetTime();
    printf("Time spent: %f ms\n", (end_time-start_time)*1000);
    pthread_mutex_destroy(&lock);
    return 0;
}