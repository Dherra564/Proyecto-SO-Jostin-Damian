#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

typedef struct {
    int id;
    float weight;
    char estate[50];
} GuiBovineInput;

typedef struct {
    int id;
    char name[50];
} GuiBuyerInput;

typedef struct {
    int bovineId;
    int buyerId;
    float pricePerKilo;
} GuiPurchaseInput;

typedef struct {
    int bovineId;
    int buyerId;
    float pricePerKilo;
} GuiEstimatePriceInput;

void guiShowWelcome(void);
int guiShowMainMenu(void);

int guiInputBovine(GuiBovineInput *data);
int guiInputBuyer(GuiBuyerInput *data);
int guiInputPurchase(GuiPurchaseInput *data);
int guiInputEstimatePrice(GuiEstimatePriceInput *data);
int guiInputReportBuyer(int *buyerId);

void guiShowBovineSuccess(void);
void guiShowBuyerSuccess(void);
void guiShowPurchaseSuccess(void);
void guiShowEstimatePriceProcessing(void);
void guiShowReportFeedback(void);
void guiShowInvalidOption(void);
void guiShowExiting(void);

#endif
