#include <stdio.h>
#include <pthread.h>
#include "controller/controller.h"
//comando compilacion: mkdir -p build && gcc -fdiagnostics-color=always -g -I src/ -I src/controller/ -I src/data/ -I src/view/ src/main.c src/controller/controller.c src/controller/guiController.c src/data/dataManager.c src/view/ui.c -o build/proyecto -pthread && ./build/proyecto
int main() {
    pthread_t controllerThread;
    
    // Lanzar hilo maestro controller
    pthread_create(&controllerThread, NULL, controllerMaster, NULL);
    
    // Esperar a que termine el controller
    pthread_join(controllerThread, NULL);

    return 0;
}
