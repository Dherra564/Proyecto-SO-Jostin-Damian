#include <stdio.h>
#include "purchase.h"

void purchaseConstructor(Purchase *p, int bovineId, int buyerId, float pricePerKilo) {
    p->bovineId = bovineId;
    p->buyerId = buyerId;
    p->pricePerKilo = pricePerKilo;
}

void showPurchase(const Purchase *p) {
    printf("Bovino ID: %d\n", p->bovineId);
    printf("Comprador ID: %d\n", p->buyerId);
    printf("Precio por kilo: $%.2f\n", p->pricePerKilo);
}
