#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "civetweb.h"
#include "functions.c"
#define MAX_CHILDREN 10

typedef struct Node {
    char *pathSegment;
    void* (*handlerFunction)(void *);
    char* paramRequirement;
    struct Node* children[MAX_CHILDREN];
    int childCount;
} Node;
Node *rootNode;

typedef struct HandlerResponse {
    void* (*handlerFunction)(void *);
    char* paramRequirement;
} HandlerResponse;

Node *initialize_node(const char *pathSegment) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->pathSegment = strdup(pathSegment);
    node->handlerFunction = root;
    node->childCount = 0;
    node->paramRequirement = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void add_route(Node *root, const char *urlPath, void * (*handler)(void *), char *paramRequirement) {
    char pathSegment[100];
    const char *position = urlPath;
    Node *currentNode = root;
    if(strcmp(position, "/") != 0){
        while (sscanf(position, "/%99[^/]", pathSegment) == 1) {
            position += strlen(pathSegment) + 1;
            int segmentFound = 0;
            for (int i = 0; i < currentNode->childCount; i++) {
                if (strcmp(currentNode->children[i]->pathSegment, pathSegment) == 0) {
                    currentNode = currentNode->children[i];
                    segmentFound = 1;
                    break;
                }
            }
            if (!segmentFound) {
                if (currentNode->childCount < MAX_CHILDREN) {
                    Node *newChildNode = initialize_node(pathSegment);
                    currentNode->children[currentNode->childCount++] = newChildNode;
                    currentNode = newChildNode;
                } else {
                    fprintf(stderr, "Max children limit reached for segment: %s\n", pathSegment);
                    return;
                }
            }
        }
    }
    currentNode->handlerFunction = handler;
    currentNode->paramRequirement = paramRequirement;
}

HandlerResponse *locate_handler(Node *root, const char *urlPath) {
    char pathSegment[100];
    const char *position = urlPath;
    if(strcmp(pathSegment, "arithmetic")==0){
        return NULL;
    }
    Node *currentNode = root;
    if(strcmp(position, "/")!=0){
        int segmentFound = 0;
        while (sscanf(position, "/%99[^/]", pathSegment) == 1) {
            position += strlen(pathSegment) + 1;
            segmentFound = 0;
            for (int i = 0; i < currentNode->childCount; i++) {
                if (strcmp(currentNode->children[i]->pathSegment, pathSegment) == 0) {
                    currentNode = currentNode->children[i];
                    segmentFound = 1;
                    break;
                }
            }
            if (!segmentFound) {
                return NULL;
            }
        }
        if(!segmentFound) return NULL;
    }
    HandlerResponse *response = (HandlerResponse *)malloc(sizeof(HandlerResponse));
    response->handlerFunction = currentNode->handlerFunction;
    response->paramRequirement = currentNode->paramRequirement;
    return response;
}

int retrieve_query_param(const char *query, const char *paramName, char *dest, size_t dest_size) {
    if(query == NULL) return 0;
    const char *start = strstr(query, paramName);
    if (start) {
        start += strlen(paramName) + 1;
        const char *end = strchr(start, '&');
        if (!end) end = start + strlen(start);
        size_t len = end - start;
        if (len >= dest_size) len = dest_size - 1;
        strncpy(dest, start, len);
        dest[len] = '\0';
        return 1;
    }
    return 0;
}

int handle_request(struct mg_connection *conn, void *cbdata) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    HandlerResponse *response = locate_handler(rootNode, request_info->request_uri);
    if(!response) {
        mg_printf(conn, "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n");
        mg_printf(conn, "No route matched the URL");
        return 400;
    }
    char output[1024];
    char paramValue[100] = {"\0"};
    int queryParamFound = 0;
    if(strcmp(response->paramRequirement, "no") != 0) {
        queryParamFound = retrieve_query_param(request_info->query_string, response->paramRequirement, paramValue, sizeof(paramValue));
    }
    if (strcmp(response->paramRequirement, "no") != 0 && !queryParamFound) {
        mg_printf(conn, "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n");
        mg_printf(conn, "Missing or invalid query parameter");
        return 400;
    }
    Request *params = (Request *)malloc(sizeof(Request));
    params->conn = conn;
    params->value = paramValue;
    (*(response->handlerFunction))(params);
    return 200;
}

void setup_routes() {
    add_route(rootNode, "/", root, "no");
    add_route(rootNode, "/square", square, "num");
    add_route(rootNode, "/arithmetic/square", square, "num");
    add_route(rootNode, "/cube", cube, "num");
    add_route(rootNode, "/arithmetic/cube", cube, "num");
    add_route(rootNode, "/hello", hello, "no");
    add_route(rootNode, "/prime", prime, "num");
    add_route(rootNode, "/arithmetic/prime", prime, "num");
    add_route(rootNode, "/pingpong", pingpong, "str");
    add_route(rootNode, "/helloworld", helloworld, "str");
    add_route(rootNode, "/arithmetic/fibonacci", fibonacci, "num");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <num_threads>\n", argv[0]);
        return 0;
    }
    rootNode = initialize_node("");
    setup_routes();
    int threadCount = atoi(argv[2]);
    const char *config[] = {
        "listening_ports", argv[1], "num_threads", argv[2], NULL
    };
    struct mg_callbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));
    struct mg_context *ctx = mg_start(&callbacks, NULL, config);
    if (ctx == NULL) {
        fprintf(stderr, "Failed to start server.\n");
        return 1;
    }
    mg_set_request_handler(ctx, "/", handle_request, NULL);
    printf("Server running on %s\n", argv[1]);
    while (1) {
        sleep(1);
    }
    mg_stop(ctx);
    return 0;
}
