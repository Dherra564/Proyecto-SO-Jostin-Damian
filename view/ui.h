#ifndef UI_H
#define UI_H

int uiReadInt(const char *message);
void uiReadString(const char *message, char *buffer, int size);
void uiPrint(const char *message);
void uiPrintInt(const char *message, int value);

#endif