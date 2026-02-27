#include <stdio.h>
#include "buyerData.h"

int writeBuyer(const char *filename, const Buyer *b) {

    FILE *file = fopen(filename, "ab"); 
    if (file == NULL) return 0;

    fwrite(b, sizeof(Buyer), 1, file);

    fclose(file);
    return 1;
}

int readBuyers(const char *filename) {

    FILE *file = fopen(filename, "rb"); 
    if (file == NULL) return 0;

    Buyer b;

    while (fread(&b, sizeof(Buyer), 1, file)) {
        showBuyer(&b); 
    }

    fclose(file);
    return 1;
}