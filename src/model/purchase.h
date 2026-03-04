#ifndef PURCHASE_H
#define PURCHASE_H

typedef struct {
    int bovineId;
    int buyerId;
    float pricePerKilo;
} Purchase;

void purchaseConstructor(Purchase *p, int bovineId, int buyerId, float pricePerKilo);
void showPurchase(const Purchase *p);

#endif
