#include <stdio.h>
#include "controller.h"
#include "../data/dataManager.h"
#include "../view/ui.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

// Estructura para pasar argumentos al hilo
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

void createBovine() {
    BovineArgs* data = malloc(sizeof(BovineArgs));

    printf("Ingrese el ID del bovino: ");
    scanf("%d", &data->bovineId);

    printf("Ingrese el peso del bovino: ");
    scanf("%f", &data->weight);

    printf("Ingrese la finca del bovino: ");
    scanf(" %49[^\n]", data->estate);

    pthread_t hilo;
    pthread_create(&hilo, NULL, saveBovine, data);
    pthread_detach(hilo);

    printf("Bovino registrado. Puedes seleccionar otra opcion\n");
}

void createBuyer() {
    BuyerArgs* data = malloc(sizeof(BuyerArgs));

    printf("Ingrese el ID del comprador: ");
    scanf("%d", &data->buyerId);

    printf("Ingrese el nombre del comprador: ");
    scanf(" %49[^\n]", data->name);

    pthread_t hilo;
    pthread_create(&hilo, NULL, saveBuyer, data);
    pthread_detach(hilo);

    printf("Comprador registrado. Puedes seleccionar otra opcion\n");
}

void* savePurchase(void* args) {
    PurchaseArgs *data = (PurchaseArgs*)args;
    dataSavePurchaseAsync(data->bovineId, data->buyerId, data->pricePerKilo);
    free(data);
    return NULL;
}

void createPurchase() {
    PurchaseArgs* data = malloc(sizeof(PurchaseArgs));

    printf("Ingrese el ID del bovino: ");
    scanf("%d", &data->bovineId);

    printf("Ingrese el ID del comprador: ");
    scanf("%d", &data->buyerId);

    printf("Ingrese el precio por kilo: $");
    scanf("%f", &data->pricePerKilo);

    pthread_t hilo;
    pthread_create(&hilo, NULL, savePurchase, data);
    pthread_detach(hilo);

    printf("Compra registrada. Puedes seleccionar otra opcion\n");
}

void* calculatePrice(void* args) {
    EstimatePriceArgs *data = (EstimatePriceArgs*)args;
    float totalPrice;
    
    dataCalculateAuction(data->bovineId, data->buyerId, data->pricePerKilo, &totalPrice);
    
    free(data);
    return NULL;
}

void estimatePrice() {
    EstimatePriceArgs *data = malloc(sizeof(EstimatePriceArgs));
    
    printf("Ingrese el ID del bovino: ");
    scanf("%d", &data->bovineId);
    
    printf("Ingrese el ID del comprador: ");
    scanf("%d", &data->buyerId);
    
    printf("Ingrese el precio por kilo: $");
    scanf("%f", &data->pricePerKilo);
    
    pthread_t hilo;
    pthread_create(&hilo, NULL, calculatePrice, data);
    pthread_detach(hilo);
    
    printf("Calculando precio... Puedes seleccionar otra opcion\n");
}

void* generateReport(void* args) {
    int *buyerId = (int*)args;
    dataReportByBuyer(*buyerId);
    free(buyerId);
    return NULL;
}

void reportBuyerPurchases() {
    int *buyerId = malloc(sizeof(int));
    
    printf("Ingrese el ID del comprador: ");
    scanf("%d", buyerId);
    
    pthread_t hilo;
    pthread_create(&hilo, NULL, generateReport, buyerId);
    pthread_join(hilo, NULL);
    
    printf("Puedes seleccionar otra opcion\n");
}

// Hilo maestro que coordina todo el sistema
void* controllerMaster(void* args) {
    dataInit();
    
    uiPrint("Bienvenido al sistema de subasta ganadera");
    int option;
    
    while(true){
        uiPrint("\n=== MENU PRINCIPAL - SUBASTA GANADERA ===");
        uiPrint("1. Registrar bovino");
        uiPrint("2. Registrar comprador");
        uiPrint("3. Registrar compra");
        uiPrint("4. Estimar precio de compra");
        uiPrint("5. Reporte de compras por comprador");
        uiPrint("6. Salir");
        scanf("%d", &option);

        switch(option) {
            case 1:
                createBovine();
                break;
            case 2:
                createBuyer();
                break;
            case 3:
                createPurchase();
                break;
            case 4:
                estimatePrice();
                break;
            case 5:
                reportBuyerPurchases();
                break;
            case 6:
                uiPrint("Saliendo del sistema...");
                dataStop();
                return NULL;
            default:
                uiPrint("Opcion no valida, intente de nuevo.");
        }
    }
}