#ifndef BUYER_H
#define BUYER_H

typedef struct {
    int id;
    char name[50];
} Buyer;

void buyerConstructor(Buyer *b, int id, const char *name);
void showBuyer(const Buyer *b);

#endif