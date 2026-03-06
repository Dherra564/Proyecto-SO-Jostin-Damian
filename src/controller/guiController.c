#include "guiController.h"
#include "../view/ui.h"
#include <string.h>

void guiShowWelcome(void) {
    uiPrint("Bienvenido al sistema de subasta ganadera");
}

int guiShowMainMenu(void) {
    uiPrint("");
    uiPrint("=== MENU PRINCIPAL - SUBASTA GANADERA ===");
    uiPrint("1. Registrar bovino");
    uiPrint("2. Registrar comprador");
    uiPrint("3. Registrar compra");
    uiPrint("4. Estimar precio de compra");
    uiPrint("5. Reporte de compras por comprador");
    uiPrint("6. Salir");
    
    int option = uiReadInt("Seleccione opcion: ");
    return option;
}

int guiInputBovine(GuiBovineInput *data) {
    if (!data) return 0;
    
    data->id = uiReadInt("Ingrese el ID del bovino: ");
    data->weight = uiReadFloat("Ingrese el peso del bovino: ");
    uiReadString("Ingrese la finca del bovino: ", data->estate, 50);
    
    return 1;
}

int guiInputBuyer(GuiBuyerInput *data) {
    if (!data) return 0;
    
    data->id = uiReadInt("Ingrese el ID del comprador: ");
    uiReadString("Ingrese el nombre del comprador: ", data->name, 50);
    
    return 1;
}

int guiInputPurchase(GuiPurchaseInput *data) {
    if (!data) return 0;
    
    data->bovineId = uiReadInt("Ingrese el ID del bovino: ");
    data->buyerId = uiReadInt("Ingrese el ID del comprador: ");
    data->pricePerKilo = uiReadFloat("Ingrese el precio por kilo: $");
    
    return 1;
}

int guiInputEstimatePrice(GuiEstimatePriceInput *data) {
    if (!data) return 0;
    
    data->bovineId = uiReadInt("Ingrese el ID del bovino: ");
    data->buyerId = uiReadInt("Ingrese el ID del comprador: ");
    data->pricePerKilo = uiReadFloat("Ingrese el precio por kilo: $");
    
    return 1;
}

int guiInputReportBuyer(int *buyerId) {
    if (!buyerId) return 0;
    
    *buyerId = uiReadInt("Ingrese el ID del comprador: ");
    return 1;
}

void guiShowBovineSuccess(void) {
    uiPrint("Bovino registrado. Puedes seleccionar otra opcion");
}

void guiShowBuyerSuccess(void) {
    uiPrint("Comprador registrado. Puedes seleccionar otra opcion");
}

void guiShowPurchaseSuccess(void) {
    uiPrint("Compra registrada. Puedes seleccionar otra opcion");
}

void guiShowEstimatePriceProcessing(void) {
    uiPrint("Calculando precio... Puedes seleccionar otra opcion");
}

void guiShowReportFeedback(void) {
    uiPrint("Puedes seleccionar otra opcion");
}

void guiShowInvalidOption(void) {
    uiPrint("Opcion no valida, intente de nuevo.");
}

void guiShowExiting(void) {
    uiPrint("Saliendo del sistema...");
}
