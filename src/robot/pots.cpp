#include <Arduino.h>
#include "pots.h"

uint16_t potReadings[NUM_POTS];
float potMags[NUM_POTS];

void potsSetup(){
    analogReadResolution(10);
    // Configure the potentiometers as inputs
    for (uint8_t i: potPins){
        pinMode(i, INPUT);
    }
}

// Function to perform oversampling and averaging
uint16_t oversampledRead(uint8_t pin) {
    long sum = 0;

    for (int i = 0; i < samples; i++) {
        sum += analogRead(pin);
    }
    // Average the readings and increase resolution
    return sum >> NUM_OVERSAMPLE_BITS; // Division by factor of 2
}

// Reads all of the potentiometers and updates the potReadings array
// Returns true if any of the potentiometers have changed by more than POT_THRESHOLD
bool readPots(){
    bool newReading = false;
    for (int i = 0; i < NUM_POTS; i++){
        #ifdef FLIP_POTS
            uint16_t newPotReading = 1023 - oversampledRead(potPins[i]);
        #else
            uint16_t newPotReading = oversampledRead(potPins[i]);
        #endif
        if (abs(newPotReading - potReadings[i]) > POT_THRESHOLD ){
            potReadings[i] = newPotReading;
            potMags[i] = float(newPotReading) / 1023.0;
            newReading = true;
        }   
    }   
    return newReading;
}

void printPots(){
    Serial.print("Pot Readings: ");
    for (uint16_t potReading: potReadings){
        Serial.printf("%d ", potReading);
    }
    Serial.println();
}

void printPotsTFT(){
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(3);
    tft.println("Pot Readings");
    tft.printf("  Kp:  %.3f\n", potMags[0]);
    tft.printf("  Ki:  %.3f\n", potMags[1]);
    tft.printf("  Kd:  %.3f\n", potMags[2]);
    tft.printf("Trim:  %.3f\n", potMags[3]);
}