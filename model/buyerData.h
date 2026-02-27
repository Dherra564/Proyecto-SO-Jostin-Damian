#ifndef BUYER_DATA_H
#define BUYER_DATA_H

#include "buyer.h"

int writeBuyer(const char *filename, const Buyer *b);
int readBuyers(const char *filename);

#endif