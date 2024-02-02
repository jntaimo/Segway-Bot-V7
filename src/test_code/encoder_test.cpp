#include "EncoderVelocity.h"
#include "pinout.h"
#include "EveryNMillis.h"


#define PRINT_DELAY 30 // Delay between printing to serial in milliseconds

// Encoder velocity readers
EncoderVelocity encoder(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN, CPR_312_RPM);


void setup() {
    Serial.begin();
}

//Prints the encoder readings to the serial monitor
void printEncoderReadings(){
    Serial.printf("encoder: Vel (rad/s): %.2f, Pos (rad): %.2f\n", 
    encoder.getVelocity(), encoder.getPosition());
 }

void loop(){
    // Print encoder readings every PRINT_DELAY milliseconds
    EVERY_N_MILLIS(PRINT_DELAY) {
        printEncoderReadings();
    }
    
}