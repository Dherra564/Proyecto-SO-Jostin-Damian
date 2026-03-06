#include "ColaRequest.h"
#include <stdlib.h>

void colaInit(ColaRequest *cola) {
    cola->head = NULL;
    cola->tail = NULL;
    pthread_mutex_init(&cola->mutex, NULL);
    pthread_cond_init(&cola->cond, NULL);
}

void colaEnqueue(ColaRequest *cola, DataRequest *req) {
    NodoCola *nodo = malloc(sizeof(NodoCola));
    nodo->request = req;
    nodo->next = NULL;
    
    pthread_mutex_lock(&cola->mutex);
    
    if (cola->tail == NULL) {
        cola->head = nodo;
        cola->tail = nodo;
    } else {
        cola->tail->next = nodo;
        cola->tail = nodo;
    }
    
    pthread_cond_signal(&cola->cond);
    pthread_mutex_unlock(&cola->mutex);
}

DataRequest* colaDequeue(ColaRequest *cola) {
    pthread_mutex_lock(&cola->mutex);
    
    while (cola->head == NULL) {
        pthread_cond_wait(&cola->cond, &cola->mutex);
    }
    
    NodoCola *nodo = cola->head;
    DataRequest *req = nodo->request;
    
    cola->head = nodo->next;
    if (cola->head == NULL) {
        cola->tail = NULL;
    }
    
    pthread_mutex_unlock(&cola->mutex);
    
    free(nodo);
    return req;
}

void colaDestroy(ColaRequest *cola) {
    pthread_mutex_destroy(&cola->mutex);
    pthread_cond_destroy(&cola->cond);
}
