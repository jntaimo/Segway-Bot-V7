//This code exists to test the functionality of the EVERY_N_MILLIS functions
//Pins should be connected to an oscilloscope to verify the timing

#include <Arduino.h>
#include "EveryNMillis.h"
#include "EveryNMicros.h"



void setup(){
    Serial.begin();
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

void loop(){
    EVERY_N_MILLIS(100){
        digitalWrite(1, HIGH);
    }
    EVERY_N_MILLIS(2){
        digitalWrite(2, HIGH);
    }
    EVERY_N_MICROS(200){
        digitalWrite(4, HIGH);
    }
    EVERY_N_MICROS(20){
        digitalWrite(5, HIGH);
    }
}