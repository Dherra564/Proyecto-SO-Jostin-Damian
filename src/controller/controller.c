#include <stdio.h>
#include "controller.h"
#include "guiController.h"
#include "../data/dataManager.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structs para pasar argumentos a los hilos
typedef struct {
    int bovineId;
    float weight;
    char estate[50];
} BovineArgs;

typedef struct {
    int buyerId;
    char name[50];
} BuyerArgs;

typedef struct {
    int bovineId;
    int buyerId;
    float pricePerKilo;
} PurchaseArgs;

typedef struct {
    int bovineId;
    float pricePerKilo;
} EstimatePriceArgs;

// HILO 1: Registrar bovino
void* threadSaveBovine(void* args) {
    BovineArgs *data = (BovineArgs*)args;
    dataSaveBovine(data->bovineId, data->weight, data->estate);
    free(data);
    return NULL;
}

// HILO 2: Registrar comprador
void* threadSaveBuyer(void* args) {
    BuyerArgs *data = (BuyerArgs*)args;
    dataSaveBuyer(data->buyerId, data->name);
    free(data);
    return NULL;
}

// HILO 3: Registrar compra
void* threadSavePurchase(void* args) {
    PurchaseArgs *data = (PurchaseArgs*)args;
    
    // Guardar compra en Compras.dat
    dataSavePurchase(data->bovineId, data->buyerId, data->pricePerKilo);
    
    // Buscar bovino, calcular y guardar en Subasta.dat
    Bovine bovine;
    if (dataSearchBovine(data->bovineId, &bovine)) {
        float totalPrice = bovine.weight * data->pricePerKilo;
        dataSaveAuction(data->bovineId, data->buyerId, totalPrice);
        
        printf("\n=== COMPRA REGISTRADA ===\n");
        printf("Bovino: %d | Peso: %.2f kg | Precio/kg: $%.2f\n", 
               bovine.id, bovine.weight, data->pricePerKilo);
        printf("Precio Total: $%.2f\n\n", totalPrice);
    } else {
        printf("Error: Bovino ID %d no encontrado.\n", data->bovineId);
    }
    
    free(data);
    return NULL;
}

// HILO 4: Estimar precio
void* threadCalculatePrice(void* args) {
    EstimatePriceArgs *data = (EstimatePriceArgs*)args;
    
    // Buscar bovino y calcular precio
    Bovine bovine;
    if (dataSearchBovine(data->bovineId, &bovine)) {
        float totalPrice = bovine.weight * data->pricePerKilo;
        dataSaveAuction(data->bovineId, -1, totalPrice);
        
        printf("\n=== ESTIMACIÓN DE PRECIO ===\n");
        printf("Bovino: %d | Peso: %.2f kg | Precio/kg: $%.2f\n", 
               bovine.id, bovine.weight, data->pricePerKilo);
        printf("Precio Total Estimado: $%.2f\n\n", totalPrice);
    } else {
        printf("Error: Bovino ID %d no encontrado.\n", data->bovineId);
    }
    
    free(data);
    return NULL;
}

// HILO 5: Generar reporte por comprador
void* threadGenerateReport(void* args) {
    int *buyerId = (int*)args;
    dataReadAuctionsByBuyer(*buyerId);
    free(buyerId);
    return NULL;
}

void* controllerMaster(void* args) {
    guiShowWelcome();
    int option;
    
    while(true) {
        option = guiShowMainMenu();

        switch(option) {
            case 1: {
                // HILO 1: Registrar bovino
                GuiBovineInput bovineInput;
                if (guiInputBovine(&bovineInput)) {
                    BovineArgs* data = malloc(sizeof(BovineArgs));
                    data->bovineId = bovineInput.id;
                    data->weight = bovineInput.weight;
                    strncpy(data->estate, bovineInput.estate, 49);
                    data->estate[49] = '\0';
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, threadSaveBovine, data);
                    pthread_detach(hilo);
                    
                    guiShowBovineSuccess();
                }
                break;
            }
            
            case 2: {
                // HILO 2: Registrar comprador
                GuiBuyerInput buyerInput;
                if (guiInputBuyer(&buyerInput)) {
                    BuyerArgs* data = malloc(sizeof(BuyerArgs));
                    data->buyerId = buyerInput.id;
                    strncpy(data->name, buyerInput.name, 49);
                    data->name[49] = '\0';
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, threadSaveBuyer, data);
                    pthread_detach(hilo);
                    
                    guiShowBuyerSuccess();
                }
                break;
            }
            
            case 3: {
                // HILO 3: Registrar compra
                GuiPurchaseInput purchaseInput;
                if (guiInputPurchase(&purchaseInput)) {
                    PurchaseArgs* data = malloc(sizeof(PurchaseArgs));
                    data->bovineId = purchaseInput.bovineId;
                    data->buyerId = purchaseInput.buyerId;
                    data->pricePerKilo = purchaseInput.pricePerKilo;
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, threadSavePurchase, data);
                    pthread_join(hilo, NULL);
                    
                    guiShowPurchaseSuccess();
                }
                break;
            }
            
            case 4: {
                // HILO 4: Estimar precio
                GuiEstimatePriceInput estimateInput;
                if (guiInputEstimatePrice(&estimateInput)) {
                    EstimatePriceArgs *data = malloc(sizeof(EstimatePriceArgs));
                    data->bovineId = estimateInput.bovineId;
                    data->pricePerKilo = estimateInput.pricePerKilo;
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, threadCalculatePrice, data);
                    pthread_detach(hilo);
                    
                    guiShowEstimatePriceProcessing();
                }
                break;
            }
            
            case 5: {
                // HILO 5: Reporte por comprador
                int buyerId;
                if (guiInputReportBuyer(&buyerId)) {
                    int *data = malloc(sizeof(int));
                    *data = buyerId;
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, threadGenerateReport, data);
                    pthread_join(hilo, NULL);
                    
                    guiShowReportFeedback();
                }
                break;
            }
            
            case 6: {
                // Salir
                guiShowExiting();
                return NULL;
            }
            
            default:
                guiShowInvalidOption();
        }
    }
}