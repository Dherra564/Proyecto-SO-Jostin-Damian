#include <stdio.h>
#include "controller.h"
#include "guiController.h"
#include "../data/dataManager.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    int buyerId;
    float pricePerKilo;
} EstimatePriceArgs;

void* saveBovine(void* args) {
    BovineArgs *data = (BovineArgs*)args;
    dataSaveBovineAsync(data->bovineId, data->weight, data->estate);
    free(data);
    return NULL;
}

void* saveBuyer(void* args) {
    BuyerArgs *data = (BuyerArgs*)args;
    dataSaveBuyerAsync(data->buyerId, data->name);
    free(data);
    return NULL;
}

void* savePurchase(void* args) {
    PurchaseArgs *data = (PurchaseArgs*)args;
    
    // Guardar la compra
    dataSavePurchaseAsync(data->bovineId, data->buyerId, data->pricePerKilo);
    
    // Calcular y guardar el precio total en Subasta.dat
    float totalPrice;
    dataCalculateAuction(data->bovineId, data->buyerId, data->pricePerKilo, &totalPrice);
    
    free(data);
    return NULL;
}

void* calculatePrice(void* args) {
    EstimatePriceArgs *data = (EstimatePriceArgs*)args;
    float totalPrice;
    
    dataCalculateAuction(data->bovineId, data->buyerId, data->pricePerKilo, &totalPrice);
    
    free(data);
    return NULL;
}

void* generateReport(void* args) {
    int *buyerId = (int*)args;
    dataReportByBuyer(*buyerId);
    free(buyerId);
    return NULL;
}

void* controllerMaster(void* args) {
    dataInit();
    
    guiShowWelcome();
    int option;
    
    while(true) {
        // Mostrar menú y obtener opción
        option = guiShowMainMenu();

        switch(option) {
            case 1: {
                // Registrar bovino
                GuiBovineInput bovineInput;
                if (guiInputBovine(&bovineInput)) {
                    BovineArgs* data = malloc(sizeof(BovineArgs));
                    data->bovineId = bovineInput.id;
                    data->weight = bovineInput.weight;
                    strncpy(data->estate, bovineInput.estate, 49);
                    data->estate[49] = '\0';
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, saveBovine, data);
                    pthread_detach(hilo);
                    
                    guiShowBovineSuccess();
                }
                break;
            }
            
            case 2: {
                // Registrar comprador
                GuiBuyerInput buyerInput;
                if (guiInputBuyer(&buyerInput)) {
                    BuyerArgs* data = malloc(sizeof(BuyerArgs));
                    data->buyerId = buyerInput.id;
                    strncpy(data->name, buyerInput.name, 49);
                    data->name[49] = '\0';
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, saveBuyer, data);
                    pthread_detach(hilo);
                    
                    guiShowBuyerSuccess();
                }
                break;
            }
            
            case 3: {
                // Registrar compra
                GuiPurchaseInput purchaseInput;
                if (guiInputPurchase(&purchaseInput)) {
                    PurchaseArgs* data = malloc(sizeof(PurchaseArgs));
                    data->bovineId = purchaseInput.bovineId;
                    data->buyerId = purchaseInput.buyerId;
                    data->pricePerKilo = purchaseInput.pricePerKilo;
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, savePurchase, data);
                    pthread_join(hilo, NULL);
                    
                    guiShowPurchaseSuccess();
                }
                break;
            }
            
            case 4: {
                // Estimar precio
                GuiEstimatePriceInput estimateInput;
                if (guiInputEstimatePrice(&estimateInput)) {
                    EstimatePriceArgs *data = malloc(sizeof(EstimatePriceArgs));
                    data->bovineId = estimateInput.bovineId;
                    data->pricePerKilo = estimateInput.pricePerKilo;
                    data->buyerId = -1;  // Placeholder, no se usa
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, calculatePrice, data);
                    pthread_detach(hilo);
                    
                    guiShowEstimatePriceProcessing();
                }
                break;
            }
            
            case 5: {
                // Reporte por comprador
                int buyerId;
                if (guiInputReportBuyer(&buyerId)) {
                    int *data = malloc(sizeof(int));
                    *data = buyerId;
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, generateReport, data);
                    pthread_join(hilo, NULL);
                    
                    guiShowReportFeedback();
                }
                break;
            }
            
            case 6: {
                // Salir
                guiShowExiting();
                dataStop();
                return NULL;
            }
            
            default:
                guiShowInvalidOption();
        }
    }
}