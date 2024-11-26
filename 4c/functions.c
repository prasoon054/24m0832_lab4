#include <stdio.h>

typedef struct {
    struct mg_connection *conn;
    char * value;
} Request;

void* root(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "Welcome to the server!\n");
    printf("Welcome to the server!\n");
    return NULL;
}

void* square(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    int num = atoi(params->value);
    if(num==0) num = 1;
    int result = num * num;
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "The square of %d is %d\n", num, result);
    printf("The square of %d is %d\n", num, result);
    return NULL;
}

void* cube(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    int num = atoi(params->value);
    if(num==0) num = 1;
    int result = num * num * num;
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "The cube of %d is %d\n", num, result);
    printf("The cube of %d is %d\n", num, result);
    return NULL;
}

void* hello(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "Hello, world!\n");
    return NULL;
}

void* prime(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    int num = atoi(params->value);
    if(num==0) num = 1;
    int is_prime = 1;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            is_prime = 0;
            break;
        }
    }
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "%d is %s\n", num, is_prime ? "prime" : "not prime");
    printf("%d is %s\n", num, is_prime ? "prime" : "not prime");
    return NULL;
}

void* pingpong(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    char * value = params->value;
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    if(strlen(value)>0) {
        mg_printf(conn, "%s\n", value);
        printf("%s\n", value);
    } else {
        mg_printf(conn, "PingPong\n");
        printf("PingPong\n");
    }
    return NULL;
}

void* helloworld(void* args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    char * value = params->value;
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    if(strlen(value)>0) {
        mg_printf(conn, "Hello, %s\n", value);
        printf("Hello, %s\n", value);
    } else {
        mg_printf(conn, "Hello\n");
        printf("Hello\n");
    }
    return NULL;
}

void* fibonacci(void * args) {
    Request *params = (Request *)args;
    struct mg_connection *conn = params->conn;
    char * value = params->value;
    int a = 0, b = 1, fib = 1;
    if(strlen(value)>0) {
        int k = atoi(value);
        if (k <= 1) fib = 1;
        else {
            for (int i = 2; i <= k; ++i) {
                fib = a + b;
                a = b;
                b = fib;
            }
        }
    }
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "%d\n", fib);
    printf("%d\n", fib);
    return NULL;
}
