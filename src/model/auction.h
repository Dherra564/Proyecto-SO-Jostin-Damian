#ifndef AUCTION_H
#define AUCTION_H

typedef struct {
    int bovineId;
    int buyerId;
    float totalPrice;
} AuctionRecord;

void auctionConstructor(AuctionRecord *a, int bovineId, int buyerId, float totalPrice);
void showAuction(const AuctionRecord *a);

#endif
