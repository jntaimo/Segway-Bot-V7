#include "EncoderVelocity.h"
#include "pinout.h"
#include "display.h"
#include "EveryNMillis.h"


// TFT display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
// Encoder velocity readers
EncoderVelocity encL(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN, CPR_1620_RPM);
EncoderVelocity encR(RIGHT_ENCODER_A_PIN, RIGHT_ENCODER_B_PIN, CPR_1620_RPM);



void setup() {
    Serial.begin();
    displaySetup();
    tft.setRotation(1);
}

//Prints the encoder readings to the serial monitor
void printEncoderReadings(){
    Serial.printf("L: Vel (rad/s): %.2f, Pos (rad): %.2f", 
    encL.getVelocity(), encL.getPosition());
    Serial.printf("   R: Vel (rad/s): %.2f, Pos (rad): %.2f\n", 
    encR.getVelocity(), encR.getPosition());
 }

void printEncoderReadingsTFT(){
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.printf("L: Vel (rad/s): %.2f\nPos (rad): %.2f\n", 
    encL.getVelocity(), encL.getPosition());
    tft.printf("R: Vel (rad/s): %.2f\nPos (rad): %.2f", 
    encR.getVelocity(), encR.getPosition());
}

void loop(){
    EVERY_N_MILLIS(1){
        encL.getVelocity();
        encR.getVelocity();
    }
    // Print encoder readings every PRINT_DELAY milliseconds
    EVERY_N_MILLIS(50) {
        printEncoderReadingsTFT();
    }
    
}