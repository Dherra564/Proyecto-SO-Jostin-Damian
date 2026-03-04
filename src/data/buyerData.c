#include <stdio.h>
#include "buyerData.h"

int writeBuyer(const char *filename, const Buyer *b) {

    FILE *file = fopen(filename, "a"); 
    if (file == NULL) return 0;

    fprintf(file, "%d,%49s\n", b->id, b->name);

    fclose(file);
    return 1;
}

int readBuyers(const char *filename) {

    FILE *file = fopen(filename, "r"); 
    if (file == NULL) return 0;

    Buyer b;

    while (fscanf(file, "%d,%49s", &b.id, b.name) == 2) {
        showBuyer(&b); 
    }

    fclose(file);
    return 1;
}