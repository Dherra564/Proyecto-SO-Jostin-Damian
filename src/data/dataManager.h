#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

// Struct para bovino
typedef struct {
    int id;
    float weight;
    char estate[50];
} Bovine;

// ESCRIBIR: Guardar datos en archivos
int dataSaveBovine(int id, float weight, const char *estate);
int dataSaveBuyer(int id, const char *name);
int dataSavePurchase(int bovineId, int buyerId, float pricePerKilo);
int dataSaveAuction(int bovineId, int buyerId, float totalPrice);

// BUSCAR: Buscar datos en archivos
int dataSearchBovine(int bovineId, Bovine *bovine);

// LEER: Leer y mostrar reportes
int dataReadAuctionsByBuyer(int buyerId);

#endif
