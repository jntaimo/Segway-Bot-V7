#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "pinout.h"
#include "pots.h"
#include "util.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup(){
    Serial.begin();
    displaySetup();
    potsSetup();
}

void loop(){
    EVERY_N_MILLIS(10){
        readPots();
    }

    EVERY_N_MILLIS(100){
        printPotsTFT();
    }
}