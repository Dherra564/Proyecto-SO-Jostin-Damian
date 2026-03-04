#include <stdio.h>
#include "controller.h"
#include "../data/bovinesData.h"
#include "../data/purchaseData.h"
#include "../data/auctionData.h"
#include "../view/ui.h"
#include <pthread.h>
#include <stdlib.h>
#include "../data/buyerData.h"
#include <stdbool.h>

// Estructura para pasar argumentos al hilo de estimacion de precio
typedef struct {
    int bovineId;
    int buyerId;
    float pricePerKilo;
} EstimatePriceArgs;


void* saveBovine(void* args) {
    Bovine *b = (Bovine*)args;
    writeBovine("bovines.dat", b);
    free(b);
    return NULL;
}

void * saveBuyer(void* args) {
    Buyer *b = (Buyer*)args;
    writeBuyer("buyers.dat", b);
    free(b);
    return NULL;
}

void createBovine() {

    Bovine* b = malloc(sizeof(Bovine));

    printf("Ingrese el ID del bovino: ");
    scanf("%d", &b->id);

    printf("Ingrese el peso del bovino: ");
    scanf("%f", &b->weight);

    printf("Ingrese la finca del bovino: ");
    scanf(" %49[^\n]", b->estate);   // ← permite espacios

    pthread_t hilo;
    pthread_create(&hilo, NULL, saveBovine, b);
    pthread_detach(hilo);

    printf("Puedes seleccionar otra opcion\n");
}

void createBuyer() {

    Buyer* b = malloc(sizeof(Buyer));

    printf("Ingrese el ID del comprador: ");
    scanf("%d", &b->id);

    printf("Ingrese el nombre del comprador: ");
    scanf(" %49[^\n]", b->name); 

    pthread_t hilo;
    pthread_create(&hilo, NULL, saveBuyer, b);
    pthread_detach(hilo);

    printf("Puedes seleccionar otra opcion\n");
}

void* savePurchase(void* args) {
    Purchase *p = (Purchase*)args;
    writePurchase("purchases.dat", p);
    free(p);
    return NULL;
}

void createPurchase() {
    Purchase* p = malloc(sizeof(Purchase));

    printf("Ingrese el ID del bovino: ");
    scanf("%d", &p->bovineId);

    printf("Ingrese el ID del comprador: ");
    scanf("%d", &p->buyerId);

    printf("Ingrese el precio por kilo: $");
    scanf("%f", &p->pricePerKilo);

    pthread_t hilo;
    pthread_create(&hilo, NULL, savePurchase, p);
    pthread_detach(hilo);

    printf("Compra registrada. Puedes seleccionar otra opcion\n");
}

void* calculatePrice(void* args) {
    EstimatePriceArgs *data = (EstimatePriceArgs*)args;
    Bovine b;
    
    // Buscar el bovino específico por ID
    if (searchBovine("bovines.dat", data->bovineId, &b)) {
        // Calcular precio total
        float totalPrice = b.weight * data->pricePerKilo;
        
        // Crear registro de subasta
        AuctionRecord auction;
        auction.bovineId = data->bovineId;
        auction.buyerId = data->buyerId;
        auction.totalPrice = totalPrice;
        
        // Guardar en Subasta.dat
        if (writeAuction("Subasta.dat", &auction)) {
            printf("\nPrecio estimado guardado exitosamente!\n");
            printf("Bovino: %d | Peso: %.2f kg | Precio/kg: $%.2f\n", 
                   b.id, b.weight, data->pricePerKilo);
            printf("Precio Total: $%.2f\n\n", totalPrice);
        } else {
            printf("Error al guardar en Subasta.dat\n");
        }
    } else {
        printf("Bovino ID %d no encontrado en bovines.dat\n", data->bovineId);
    }
    
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
    readAuctionsByBuyer("Subasta.dat", *buyerId);
    free(buyerId);
    return NULL;
}

void reportBuyerPurchases() {
    int *buyerId = malloc(sizeof(int));
    
    printf("Ingrese el ID del comprador: ");
    scanf("%d", buyerId);
    
    pthread_t hilo;
    pthread_create(&hilo, NULL, generateReport, buyerId);
    pthread_join(hilo, NULL);  // Esperar a que termine para mostrar el reporte
    
    printf("Puedes seleccionar otra opcion\n");
}

void initController() {
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
                return;
            default:
                uiPrint("Opcion no valida, intente de nuevo.");
        }
    }
}