#include <stdio.h>
#include <string.h>
#include "buyer.h"

void buyerConstructor(Buyer *b, int id, const char *name){
    b->id = id;
    strcpy(b->name, name);
}

void showBuyer(const Buyer *b){
    printf("ID: %d\n", b->id);
    printf("Name: %s\n", b->name);
}
