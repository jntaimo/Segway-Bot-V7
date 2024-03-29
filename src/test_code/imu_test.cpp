#include <Arduino.h>
#include "imu.h"
#include "EulerAngles.h"
#include "util.h"
#include "pinout.h"
#include "display.h"

#define DISPLAY_DELAY 50 //ms between display updates

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
IMU imu(IMU_RST, IMU_CS, IMU_INT);

void setup() {
    imu.setup();
    displaySetup();
    tft.setTextSize(2);
    tft.setTextWrap(false); //Allow text to run off screen  
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);    
    tft.setCursor(0, 0);
    tft.println("Euler Angles (deg):");  
    tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);  
}

void loop() {
    //update IMU to check for new reading quickly
    EVERY_N_MICROS(500){
        imu.readIMU();
    }
    //print Euler angles at 10 Hz
    EVERY_N_MILLIS(DISPLAY_DELAY) {
        //read the current angles
        EulerAngles angles = imu.getEulerAngles();

        tft.setCursor(0, 30);
        tft.printf(" Roll: %.2f    \n", angles.roll*RAD_2_DEG);
        // tft.printf("Pitch: %.2f    \n", angles.pitch*RAD_2_DEG);
        // tft.printf("  Yaw: %.2f    \n", angles.yaw*RAD_2_DEG);
        // tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        // tft.println("Gyro Readings (deg/s):");
        // tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
        // tft.printf(" RollRate: %.2f      \n", gyro.rollRate*RAD_2_DEG);
        // tft.printf("PitchRate: %.2f      \n", gyro.pitchRate*RAD_2_DEG);
        // tft.printf("  YawRate: %.2f      \n", gyro.yawRate*RAD_2_DEG);
        tft.printf("\nTime (ms): %d      \n", millis());
    }
}

