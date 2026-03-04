#ifndef BOVINE_DATA_H
#define BOVINE_DATA_H

#include "../model/bovine.h"

int writeBovine(const char *filename, const Bovine *b);
int readBovine(const char *filename, Bovine *b);
int searchBovine(const char *filename, int id, Bovine *b);

#endif