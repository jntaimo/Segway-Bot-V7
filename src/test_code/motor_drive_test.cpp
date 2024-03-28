#include <Arduino.h>
#include "pinout.h"
#include "MotorDriver.h"

#define DELAY 1000 // Delay between motor movements in milliseconds


// Create an instance of the MotorDriver class
MotorDriver motorL(DIR1, PWM1, 0);
MotorDriver motorR(DIR2, PWM2, 1);

void setup() {
    // Initialize serial communication
    Serial.begin();

    // Setup the motor driver
    motorL.setup();
    motorR.setup();
}

void loop() {
    // Move the motor forward at full speed
    Serial.println("Moving Forward at full speed");
    motorL.drive(1.0); // 100% duty cycle
    motorR.drive(1.0); 
    delay(DELAY);

    // Stop the motor
    Serial.println("Stopping");
    motorL.drive(0.0); // 0% duty cycle
    motorR.drive(0.0);
    delay(DELAY);

    // Move the motor forward at half speed
    Serial.println("Moving Forward at half speed");
    motorL.drive(0.5); // 50% duty cycle
    motorR.drive(0.5);
    delay(DELAY);

    // Stop the motor
    Serial.println("Stopping");
    motorL.drive(0.0); // 0% duty cycle
    motorR.drive(0.0);
    delay(DELAY);

    // Move the motor backward at full speed
    Serial.println("Moving Backward at full speed");
    motorL.drive(-1.0); // -100% duty cycle (backward)
    motorR.drive(-1.0);
    delay(DELAY);

    // Stop the motor
    Serial.println("Stopping");
    motorL.drive(0.0); // 0% duty cycle
    motorR.drive(0.0);
    delay(DELAY);

    // Move the motor backward at half speed
    Serial.println("Moving Backward at half speed");
    motorL.drive(-0.5); // -50% duty cycle (backward)
    motorR.drive(-0.5);
    delay(DELAY);

    // Stop the motor
    Serial.println("Stopping");
    motorL.drive(0.0); // 0% duty cycle
    motorR.drive(0.0);
    delay(DELAY);

}
