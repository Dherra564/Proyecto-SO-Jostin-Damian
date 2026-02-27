#ifndef BOVINE_H
#define BOVINE_H

typedef struct {
    int id;
    float weight;
    char estate[50];
} Bovine;

void bovineConstructor(Bovine *b, int id, float weight, const char *estate);
void showBovine(const Bovine *b);

#endif