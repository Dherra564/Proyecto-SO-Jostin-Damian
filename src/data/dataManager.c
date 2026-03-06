#include "dataManager.h"
#include "../utils/ColaRequest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Definición local del struct Bovine
typedef struct {
    int id;
    float weight;
    char estate[50];
} Bovine;

static ColaRequest cola;
static pthread_t workerThread;
static int running = 0;

// Funciones internas de I/O
static int searchBovineInFile(const char *filename, int id, Bovine *b) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    Bovine temp;
    while (fscanf(file, "%d,%f,%49[^\n]\n", &temp.id, &temp.weight, temp.estate) == 3) {
        if (temp.id == id) {
            *b = temp;
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

static int saveBovineToFile(const char *filename, int id, float weight, const char *estate) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%.2f,%s\n", id, weight, estate);
    fclose(file);
    return 1;
}

static int saveBuyerToFile(const char *filename, int id, const char *name) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%s\n", id, name);
    fclose(file);
    return 1;
}

static int savePurchaseToFile(const char *filename, int bovineId, int buyerId, float pricePerKilo) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%d,%.2f\n", bovineId, buyerId, pricePerKilo);
    fclose(file);
    return 1;
}

static int saveAuctionToFile(const char *filename, int bovineId, int buyerId, float totalPrice) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%d,%.2f\n", bovineId, buyerId, totalPrice);
    fclose(file);
    return 1;
}

static int readAuctionsByBuyerFromFile(const char *filename, int buyerId) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se encontraron registros de subasta.\n");
        return 0;
    }
    
    int bovineId, buyerIdFile;
    float totalPrice;
    int count = 0;
    float totalSpent = 0;
    
    printf("\n=== SUBASTAS DEL COMPRADOR %d ===\n", buyerId);
    
    while (fscanf(file, "%d,%d,%f\n", &bovineId, &buyerIdFile, &totalPrice) == 3) {
        if (buyerIdFile == buyerId) {
            printf("Bovino ID: %d | Precio Total: $%.2f\n", bovineId, totalPrice);
            totalSpent += totalPrice;
            count++;
        }
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No se encontraron subastas para este comprador.\n");
    } else {
        printf("\nTotal de bovinos comprados: %d\n", count);
        printf("Gasto total: $%.2f\n\n", totalSpent);
    }
    
    return count;
}

// Worker thread
static void* dataWorker(void* arg) {
    while (running) {
        DataRequest *req = colaDequeue(&cola);
        
        if (req->op == OP_SHUTDOWN) {
            free(req);
            break;
        }
        
        switch (req->op) {
            case OP_SAVE_BOVINE:
                req->result = saveBovineToFile("Bovinos.dat", req->bovineId, req->weight, req->estate);
                break;
                
            case OP_SAVE_BUYER:
                req->result = saveBuyerToFile("Compradores.dat", req->buyerId, req->name);
                break;
                
            case OP_SAVE_PURCHASE:
                req->result = savePurchaseToFile("Compras.dat", req->bovineId, req->buyerId, req->pricePerKilo);
                break;
                
            case OP_CALCULATE_AUCTION: {
                Bovine b;
                if (searchBovineInFile("Bovinos.dat", req->bovineId, &b)) {
                    req->totalPrice = b.weight * req->pricePerKilo;
                    req->result = saveAuctionToFile("Subasta.dat", req->bovineId, req->buyerId, req->totalPrice);
                    
                    if (req->result) {
                        printf("\nPrecio estimado guardado exitosamente!\n");
                        printf("Bovino: %d | Peso: %.2f kg | Precio/kg: $%.2f\n", 
                               b.id, b.weight, req->pricePerKilo);
                        printf("Precio Total: $%.2f\n\n", req->totalPrice);
                    }
                } else {
                    printf("Bovino ID %d no encontrado en Bovinos.dat\n", req->bovineId);
                    req->result = 0;
                }
                break;
            }
                
            case OP_REPORT_BY_BUYER:
                req->result = readAuctionsByBuyerFromFile("Subasta.dat", req->buyerId);
                break;
                
            default:
                req->result = 0;
                break;
        }
        
        // Si es operación síncrona, notificar
        if (req->op == OP_CALCULATE_AUCTION || req->op == OP_REPORT_BY_BUYER) {
            pthread_mutex_lock(&req->doneMutex);
            req->done = 1;
            pthread_cond_signal(&req->doneCond);
            pthread_mutex_unlock(&req->doneMutex);
        } else {
            free(req);
        }
    }
    
    return NULL;
}

// API pública
void dataInit(void) {
    colaInit(&cola);
    running = 1;
    pthread_create(&workerThread, NULL, dataWorker, NULL);
}

void dataStop(void) {
    running = 0;
    
    DataRequest *req = malloc(sizeof(DataRequest));
    req->op = OP_SHUTDOWN;
    colaEnqueue(&cola, req);
    
    pthread_join(workerThread, NULL);
    colaDestroy(&cola);
}

void dataSaveBovineAsync(int id, float weight, const char *estate) {
    DataRequest *req = malloc(sizeof(DataRequest));
    req->op = OP_SAVE_BOVINE;
    req->bovineId = id;
    req->weight = weight;
    strncpy(req->estate, estate, 49);
    req->estate[49] = '\0';
    
    colaEnqueue(&cola, req);
}

void dataSaveBuyerAsync(int id, const char *name) {
    DataRequest *req = malloc(sizeof(DataRequest));
    req->op = OP_SAVE_BUYER;
    req->buyerId = id;
    strncpy(req->name, name, 49);
    req->name[49] = '\0';
    
    colaEnqueue(&cola, req);
}

void dataSavePurchaseAsync(int bovineId, int buyerId, float pricePerKilo) {
    DataRequest *req = malloc(sizeof(DataRequest));
    req->op = OP_SAVE_PURCHASE;
    req->bovineId = bovineId;
    req->buyerId = buyerId;
    req->pricePerKilo = pricePerKilo;
    
    colaEnqueue(&cola, req);
}

int dataCalculateAuction(int bovineId, int buyerId, float pricePerKilo, float *totalPrice) {
    DataRequest *req = malloc(sizeof(DataRequest));
    req->op = OP_CALCULATE_AUCTION;
    req->bovineId = bovineId;
    req->buyerId = buyerId;
    req->pricePerKilo = pricePerKilo;
    req->done = 0;
    
    pthread_mutex_init(&req->doneMutex, NULL);
    pthread_cond_init(&req->doneCond, NULL);
    
    colaEnqueue(&cola, req);
    
    // Esperar resultado
    pthread_mutex_lock(&req->doneMutex);
    while (!req->done) {
        pthread_cond_wait(&req->doneCond, &req->doneMutex);
    }
    pthread_mutex_unlock(&req->doneMutex);
    
    int result = req->result;
    if (totalPrice && result) {
        *totalPrice = req->totalPrice;
    }
    
    pthread_mutex_destroy(&req->doneMutex);
    pthread_cond_destroy(&req->doneCond);
    free(req);
    
    return result;
}

int dataReportByBuyer(int buyerId) {
    DataRequest *req = malloc(sizeof(DataRequest));
    req->op = OP_REPORT_BY_BUYER;
    req->buyerId = buyerId;
    req->done = 0;
    
    pthread_mutex_init(&req->doneMutex, NULL);
    pthread_cond_init(&req->doneCond, NULL);
    
    colaEnqueue(&cola, req);
    
    // Esperar resultado
    pthread_mutex_lock(&req->doneMutex);
    while (!req->done) {
        pthread_cond_wait(&req->doneCond, &req->doneMutex);
    }
    pthread_mutex_unlock(&req->doneMutex);
    
    int result = req->result;
    
    pthread_mutex_destroy(&req->doneMutex);
    pthread_cond_destroy(&req->doneCond);
    free(req);
    
    return result;
}
