#ifndef POTS_H
#define POTS_H
#include <Arduino.h>
#include "pinout.h"
#include "display.h"

#define NUM_POTS 4
#define POT_THRESHOLD 1
#define NUM_OVERSAMPLE_BITS 6
//#define FLIP_POTS
const int samples = 1 << NUM_OVERSAMPLE_BITS; // Number of samples for oversampling
const uint8_t potPins[] = {POT_1_PIN, POT_2_PIN, POT_3_PIN, POT_4_PIN};

bool readPots();
void printPots();
void potsSetup();
void printPotsTFT();
extern uint16_t potReadings[NUM_POTS];
extern float potMags[NUM_POTS];
#endif // POTS_H