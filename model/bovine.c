#include <stdio.h>
#include <string.h>
#include "bovine.h"

void bovineConstructor(Bovine *b, int id, float weight, const char *estate){
    b->id = id;
    b->weight = weight;
    strcpy(b->estate, estate);
}

void showBovine(const Bovine *b) {
    printf("ID: %d\n", b->id);
    printf("Weight: %.2f\n", b->weight);
    printf("Estate: %s\n", b->estate);
}