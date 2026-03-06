#include "dataManager.h"
#include <stdio.h>
#include <string.h>

// ESCRIBIR: Guardar bovino en Bovinos.dat
int dataSaveBovine(int id, float weight, const char *estate) {
    FILE *file = fopen("Bovinos.dat", "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%.2f,%s\n", id, weight, estate);
    fclose(file);
    return 1;
}

// ESCRIBIR: Guardar comprador en Compradores.dat
int dataSaveBuyer(int id, const char *name) {
    FILE *file = fopen("Compradores.dat", "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%s\n", id, name);
    fclose(file);
    return 1;
}

// ESCRIBIR: Guardar compra en Compras.dat
int dataSavePurchase(int bovineId, int buyerId, float pricePerKilo) {
    FILE *file = fopen("Compras.dat", "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%d,%.2f\n", bovineId, buyerId, pricePerKilo);
    fclose(file);
    return 1;
}

// ESCRIBIR: Guardar subasta en Subasta.dat
int dataSaveAuction(int bovineId, int buyerId, float totalPrice) {
    FILE *file = fopen("Subasta.dat", "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d,%d,%.2f\n", bovineId, buyerId, totalPrice);
    fclose(file);
    return 1;
}

// BUSCAR: Buscar bovino por ID en Bovinos.dat
int dataSearchBovine(int bovineId, Bovine *bovine) {
    FILE *file = fopen("Bovinos.dat", "r");
    if (file == NULL) return 0;
    
    Bovine temp;
    while (fscanf(file, "%d,%f,%49[^\n]\n", &temp.id, &temp.weight, temp.estate) == 3) {
        if (temp.id == bovineId) {
            *bovine = temp;
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

// LEER: Leer y mostrar subastas por comprador
int dataReadAuctionsByBuyer(int buyerId) {
    FILE *file = fopen("Subasta.dat", "r");
    if (file == NULL) {
        printf("No se encontraron registros de subasta.\n");
        return 0;
    }
    
    int bovineId, buyerIdFile;
    float totalPrice;
    int count = 0;
    float totalSpent = 0;
    
    printf("\n========================================\n");
    printf("   REPORTE DE COMPRAS - COMPRADOR %d\n", buyerId);
    printf("========================================\n\n");
    
    while (fscanf(file, "%d,%d,%f\n", &bovineId, &buyerIdFile, &totalPrice) == 3) {
        if (buyerIdFile == buyerId) {
            printf("  Bovino ID: %d | Precio Total: $%.2f\n", bovineId, totalPrice);
            totalSpent += totalPrice;
            count++;
        }
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("  No se encontraron compras.\n");
    } else {
        printf("\n----------------------------------------\n");
        printf("  Total de bovinos: %d\n", count);
        printf("  Gasto total: $%.2f\n", totalSpent);
    }
    printf("========================================\n\n");
    
    return count;
}
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
