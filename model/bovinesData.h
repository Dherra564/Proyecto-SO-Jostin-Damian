#ifndef BOVINE_DATA_H
#define BOVINE_DATA_H

#include "bovine.h"

int writeBovine(const char *filename, const Bovine *b);
int readBovine(const char *filename, Bovine *b);

#endif