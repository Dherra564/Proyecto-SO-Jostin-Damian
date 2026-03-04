#include <stdio.h>
#include "auctionData.h"

int writeAuction(const char *filename, const AuctionRecord *a) {
    FILE *file = fopen(filename, "a");
    
    if (file == NULL) {
        return 0;  // error
    }
    
    fprintf(file, "%d,%d,%.2f\n", a->bovineId, a->buyerId, a->totalPrice);
    
    fclose(file);
    return 1;  // success
}

int readAuctionsByBuyer(const char *filename, int buyerId) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        return 0;  // error
    }
    
    AuctionRecord a;
    int count = 0;
    float totalSpent = 0;
    
    printf("\n=== SUBASTAS DEL COMPRADOR %d ===\n", buyerId);
    
    while (fscanf(file, "%d,%d,%f", &a.bovineId, &a.buyerId, &a.totalPrice) == 3) {
        if (a.buyerId == buyerId) {
            printf("Bovino ID: %d | Precio Total: $%.2f\n", a.bovineId, a.totalPrice);
            totalSpent += a.totalPrice;
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
