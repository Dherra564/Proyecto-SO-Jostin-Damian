#include <stdio.h>
#include "auction.h"

void auctionConstructor(AuctionRecord *a, int bovineId, int buyerId, float totalPrice) {
    a->bovineId = bovineId;
    a->buyerId = buyerId;
    a->totalPrice = totalPrice;
}

void showAuction(const AuctionRecord *a) {
    printf("Bovino ID: %d\n", a->bovineId);
    printf("Comprador ID: %d\n", a->buyerId);
    printf("Precio Total: $%.2f\n", a->totalPrice);
}
