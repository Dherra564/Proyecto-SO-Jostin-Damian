#include <stdio.h>
#include "bovinesData.h"

int writeBovine(const char *filename, const Bovine *b) {

    FILE *file = fopen(filename, "ab");  // append binary

    if (file == NULL) {
        return 0;  // error
    }

    fwrite(b, sizeof(Bovine), 1, file);

    fclose(file);

    return 1;  // success
}


int readBovine(const char *filename, Bovine *b) {

    FILE *file = fopen(filename, "rb");  // read binary

    if (file == NULL) {
        return 0;  // error
    }

    size_t result = fread(b, sizeof(Bovine), 1, file);

    fclose(file);

    return result;  // 1 si leyó correctamente
}