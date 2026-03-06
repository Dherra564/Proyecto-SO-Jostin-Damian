#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

void dataInit(void);
void dataStop(void);

void dataSaveBovineAsync(int id, float weight, const char *estate);
void dataSaveBuyerAsync(int id, const char *name);
void dataSavePurchaseAsync(int bovineId, int buyerId, float pricePerKilo);
int dataCalculateAuction(int bovineId, int buyerId, float pricePerKilo, float *totalPrice);
int dataReportByBuyer(int buyerId);

#endif
