#ifndef AUCTION_DATA_H
#define AUCTION_DATA_H

#include "../model/auction.h"

int writeAuction(const char *filename, const AuctionRecord *a);
int readAuctionsByBuyer(const char *filename, int buyerId);

#endif
