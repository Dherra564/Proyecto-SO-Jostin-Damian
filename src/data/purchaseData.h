#ifndef PURCHASE_DATA_H
#define PURCHASE_DATA_H

#include "../model/purchase.h"

int writePurchase(const char *filename, const Purchase *p);
int readPurchasesByBuyer(const char *filename, int buyerId);

#endif
