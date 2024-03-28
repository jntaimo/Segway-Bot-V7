#include "display.h"

void displaySetup(){
    tft.init(240, 320);           // Init ST7789 320x240
    tft.fillScreen(ST77XX_BLACK);
}