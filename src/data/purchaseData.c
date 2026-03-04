#include <stdio.h>
#include "purchaseData.h"

int writePurchase(const char *filename, const Purchase *p) {
    FILE *file = fopen(filename, "a");
    
    if (file == NULL) {
        return 0;  // error
    }
    
    fprintf(file, "%d,%d,%.2f\n", p->bovineId, p->buyerId, p->pricePerKilo);
    
    fclose(file);
    return 1;  // success
}

int readPurchasesByBuyer(const char *filename, int buyerId) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        return 0;  // error
    }
    
    Purchase p;
    int count = 0;
    
    printf("\n=== COMPRAS DEL COMPRADOR %d ===\n", buyerId);
    
    while (fscanf(file, "%d,%d,%f", &p.bovineId, &p.buyerId, &p.pricePerKilo) == 3) {
        if (p.buyerId == buyerId) {
            printf("Bovino ID: %d | Precio/kg: $%.2f\n", p.bovineId, p.pricePerKilo);
            count++;
        }
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No se encontraron compras para este comprador.\n");
    } else {
        printf("\nTotal de compras: %d\n", count);
    }
    
    return count;
}
