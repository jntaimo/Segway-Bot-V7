#ifndef POTS_H
#define POTS_H
#include <Arduino.h>
#include "pinout.h"
#include "display.h"
bool readPots();
void printPots();
void potsSetup();
void printPotsTFT();
extern uint16_t potReadings[4];
#endif // POTS_H