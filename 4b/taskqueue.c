#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct Task{
    char type;
    long burst_time;
    struct Task* next;
} Task;

Task *head, *tail;

void enqueue(char type, long burst_time){
    Task* tsk = (Task*)malloc(sizeof(Task));
    tsk->type = type;
    tsk->burst_time = burst_time;
    tsk->next = NULL;
    if(tail){
        tail->next = tsk;
        tail = tsk;
    }
    else{
        head = tsk;
        tail = tsk;
    }
}

Task* dequeue(){
    if(!head) return NULL;
    Task* res = head;
    head = head->next;
    if(!head){
        tail = NULL;
    }
    return res;
}

// void* processtask(void* num);

void* processtask(long number){
    // simulate burst time
    // long number = *(long*)num;
    sleep(number);

    // update global variables
    pthread_mutex_lock(&lock);
    sum += number;
    if (number % 2 == 1){
        odd++;
    }
    else{
        even++;
    }
    if (number < min){
        min = number;
    }
    if (number > max){
        max = number;
    }
    pthread_mutex_unlock(&lock);
}

void* sleeptask(void* num){
    long number = *(long*)num;
    sleep(number);
}

void* worker_thread(void* arg){
    while(1){
        pthread_mutex_lock(&lock);
        while(!head && !done){
            pthread_cond_wait(&cond, &lock);
        }
        if(!head && done){
            // p(&lock);
            pthread_mutex_unlock(&lock);
            break;
        }
        Task* tsk = dequeue();
        pthread_mutex_unlock(&lock);
        if(tsk){
            if(tsk->type=='p'){
                processtask(tsk->burst_time);
                printf("Task completed\n");
            }
            else if(tsk->type=='w'){
                sleep(tsk->burst_time);
                printf("Wait over\n");
            }
            free(tsk);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Usage: %s <tasklist_file> <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    int num_th = atoi(argv[2]);
    char type;
    long num;
    pthread_t th[num_th];
    for(int i=0; i<num_th; i++){
        if(pthread_create(th+i, NULL, worker_thread, NULL)!=0){
            return -1;
        }
    }

    // pthread_mutex_init(&lock);
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2){
        pthread_mutex_lock(&lock);
        enqueue(type, num);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    fclose(fin);
    pthread_mutex_lock(&lock);
    done = true;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    for(int i=0; i<num_th; i++){
        if(pthread_join(th[i], NULL)!=0){
            return 2;
        }
        // printf("Transaction %d has finished\n", i);
    }
    // Print global variables
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);
    pthread_mutex_destroy(&lock);
    return (EXIT_SUCCESS);
}
