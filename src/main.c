#include <stdio.h>
#include <pthread.h>
#include "controller/controller.h"

int main() {
    pthread_t controllerThread;
    
    // Lanzar hilo maestro controller
    pthread_create(&controllerThread, NULL, controllerMaster, NULL);
    
    // Esperar a que termine el controller
    pthread_join(controllerThread, NULL);

    return 0;
}
