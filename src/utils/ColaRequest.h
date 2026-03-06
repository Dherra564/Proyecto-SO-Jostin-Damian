#ifndef COLA_REQUEST_H
#define COLA_REQUEST_H

#include <pthread.h>

typedef enum {
    OP_SAVE_BOVINE,
    OP_SAVE_BUYER,
    OP_SAVE_PURCHASE,
    OP_CALCULATE_AUCTION,
    OP_REPORT_BY_BUYER,
    OP_SHUTDOWN
} DataOp;

typedef struct {
    DataOp op;
    
    // Datos de entrada
    int bovineId;
    int buyerId;
    float weight;
    float pricePerKilo;
    char estate[50];
    char name[50];
    
    // Resultado
    int result;  // 1=éxito, 0=error
    float totalPrice;
    
    // Sincronización para operaciones síncronas
    pthread_mutex_t doneMutex;
    pthread_cond_t doneCond;
    int done;
} DataRequest;

typedef struct NodoCola {
    DataRequest *request;
    struct NodoCola *next;
} NodoCola;

typedef struct {
    NodoCola *head;
    NodoCola *tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ColaRequest;

void colaInit(ColaRequest *cola);
void colaEnqueue(ColaRequest *cola, DataRequest *req);
DataRequest* colaDequeue(ColaRequest *cola);
void colaDestroy(ColaRequest *cola);

#endif
