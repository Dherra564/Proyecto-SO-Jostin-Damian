#include <stdio.h>
#include <string.h>
#include "ui.h"

static void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int uiReadInt(const char *message) {
    int value;
    printf("%s", message);
    scanf("%d", &value);
    clearBuffer();
    return value;
}

void uiReadString(const char *message, char *buffer, int size) {
    printf("%s", message);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // quitar salto de línea
}

void uiPrint(const char *message) {
    printf("%s\n", message);
}

void uiPrintInt(const char *message, int value) {
    printf("%s%d\n", message, value);
}