#include <stdio.h>
#include "controller.h"
#include "../model/bovinesData.h"
#include "../view/ui.h"
#include <pthread.h>
#include <stdlib.h>
#include "../model/buyerData.h"
#include <stdbool.h>


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

void initController() {
    uiPrint("Bienvenido al sistema de subasta ganadera");
    int option;
    
    while(true){
        uiPrint("Seleccione una opcion:");
        uiPrint("1. Agregar bovino");
        uiPrint("2. Agregar comprador");
        uiPrint("3. Salir");
        scanf("%d", &option);

        switch(option) {
            case 1:
                createBovine();
                break;
            case 2:
                createBuyer();
                break;
            case 3:
                uiPrint("Saliendo del sistema...");
                return;
            default:
                uiPrint("Opcion no valida, intente de nuevo.");
        }
    }
}