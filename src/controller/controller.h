#ifndef CONTROLLER_H
#define CONTROLLER_H

void* saveBovine(void* args);
void* saveBuyer(void* args);
void* savePurchase(void* args);
void* calculatePrice(void* args);
void* generateReport(void* args);
void createBovine();
void createBuyer();
void createPurchase();
void estimatePrice();
void reportBuyerPurchases();
void initController();

#endif