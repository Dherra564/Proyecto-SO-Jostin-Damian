#include <stdio.h>
#include "bovinesData.h"

int writeBovine(const char *filename, const Bovine *b) {

    FILE *file = fopen(filename, "a");  // append text

    if (file == NULL) {
        return 0;  // error
    }

    fprintf(file, "%d,%.2f,%s\n", b->id, b->weight, b->estate);

    fclose(file);

    return 1;  // success
}


int readBovine(const char *filename, Bovine *b) {

    FILE *file = fopen(filename, "r");  // read text

    if (file == NULL) {
        return 0;  // error
    }

    int result = fscanf(file, "%d,%f,%49s", &b->id, &b->weight, b->estate);

    fclose(file);

    return (result == 3) ? 1 : 0;  // 1 si leyó correctamente
}