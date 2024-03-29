#include "imu.h"
#include "PID.h"
#include "pots.h"
#include "util.h"
#include "balance.h"
#include "pinout.h"
#include "EncoderVelocity.h"
#include "display.h"
#include "MotorDriver.h"
#include <UMS3.h>

//whether the robot has fallen over
bool bonked = false;

//PID Parameters
float balanceSetpoint = 0; //rad
float motorLSetpoint = 0;  //rad/s
float motorRSetpoint = 0;  //rad/s
float observerSetpoint = 0; //rad
float turnSpeed = 0; //rad/s

float balanceControlEffort = 0; //duty cycle
float motorLControlEffort = 0; //duty cycle
float motorRControlEffort = 0; //duty cycle
float observerControlEffort = 0; //duty cycle

//Gains in kp ki kd trim
Gains balanceGains({0, 0, 0, 0});
Gains motorGains({0, 0, 0, 0});
Gains observerGains({0, 0, 0, 0});

// PID controllers
PID balancePID(balanceGains.kp, balanceGains.ki, balanceGains.kd, 0, 0.05, false);  
PID leftMotorPID(motorGains.kp, motorGains.ki, motorGains.kd, 0.0, 0.1, false); 
PID rightMotorPID(observerGains.kp, observerGains.ki, observerGains.kd, 0.0, 0.1, false); 

// Encoder velocity readers
EncoderVelocity leftEncoder(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN, CPR_1620_RPM);
EncoderVelocity rightEncoder(RIGHT_ENCODER_A_PIN, RIGHT_ENCODER_B_PIN, CPR_1620_RPM);

// Motor drivers
MotorDriver leftMotor(DIR1, PWM1, 0);
MotorDriver rightMotor(DIR2, PWM2, 1);

// Inertial Measurement Unit for measuring angle
IMU imu(IMU_RST, IMU_CS, IMU_INT);

// TFT display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//tft print buffer
char buf[11];

void setup(){
    Serial.begin();
    imu.setup();
    displaySetup();
    potsSetup();
    leftMotor.setup();
    rightMotor.setup();
    tft.setTextWrap(false);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(2);

}

void loop(){
    //update the setponts at 100 Hz
    EVERY_N_MILLIS(100){
        balanceSetpoint = 0;
        turnSpeed = 0;
    }
    //update gains at 5 Hz
    EVERY_N_MILLIS(200){
        //update 
        bool newPotReading  = updatePIDParams(balanceGains);
        if (newPotReading){
            balancePID.setParallelTunings(balanceGains.kp, balanceGains.ki, balanceGains.kd, 0.02, -0.5, 0.5);
        }
    }

    //update IMU at 1kHz
    EVERY_N_MILLIS(1){
        imu.update();
    }
    
    // Update PID at 2kHz
    EVERY_N_MILLIS(1){
        
        //read encoders
        float leftPosition = leftEncoder.getPosition(); //rad
        float rightPosition = -rightEncoder.getPosition(); //rad
        float leftVelocity = leftEncoder.getVelocity(); //rad
        float rightVelocity = -rightEncoder.getVelocity(); //rad

        //calculate control effort
        EulerAngles angles = imu.getEulerAngles();
        GyroReadings gyro = imu.getGyroReadings();

        float absoluteTiltAngle = angles.roll;
        float relativeTiltAngle = absoluteTiltAngle - balanceGains.trim;
        bonked = !safeAngle(absoluteTiltAngle, balanceGains.trim, MAX_TILT*DEG_TO_RAD);

        //turn off the controller when the tilt angle is too high
        //this is useful for preventing integral wind up 
        if (bonked){
            //cut out the motor for safety!
            leftMotor.drive(0);
            rightMotor.drive(0);
            //don't update the PID controllers at all!
        }
        else{
            balanceControlEffort = balancePID.calculateParallel(relativeTiltAngle, balanceSetpoint, -gyro.rollRate); //desired average motor speed
            
            //measure the average position so that that the observer can change the trim
            // float avergagePosition = (leftPosition + rightPosition) / 2;
            // observerControlEffort = rightMotorPID.calculateParallel(observerSetpoint, avergagePosition);//amount to change trim by
            
            motorLSetpoint = (balanceControlEffort - turnSpeed); //left motor desired speed
            motorRSetpoint = (balanceControlEffort + turnSpeed); //right motor desired speed
            
            // motorLControlEffort = leftMotorPID.calculateParallel(motorLSetpoint, leftVelocity); //left motor duty cycle
            // motorRControlEffort = rightMotorPID.calculateParallel(motorRSetpoint, rightVelocity); //right motor duty cycle
            
            //drive the motors
            leftMotor.drive(motorLSetpoint);
            rightMotor.drive(-motorRSetpoint);
        }
    }

    // Print values at 10Hz
    EVERY_N_MILLIS(100) {
        Serial.printf("kp: %.2f, ki: %.2f, kd: %.2f, trim: %.2f", balanceGains.kp, balanceGains.ki, balanceGains.kd, balanceGains.trim);
        Serial.printf(" Setpoint: %.2f, Angle: %.2f, Control Effort: %.2f\n", balanceSetpoint, imu.getEulerAngles().roll*RAD_2_DEG, balanceControlEffort);
        // Print encoder readings
        // tft.setCursor(0, 0);
        // tft.println("Pot Readings");
        // tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
        // tft.printf("  Kp:  %.3f", balanceGains.kp);
        // tft.print("         \n");
        // tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
        // tft.printf("  Ki:  %.3f", balanceGains.ki);
        // tft.print("         \n");
        // tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
        // tft.printf("  Kd:  %.3f", balanceGains.kd);
        // tft.print("         \n");
        // tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        // tft.printf("Trim:  %.2f", balanceGains.trim);
        // tft.print("         \n\n");
        // tft.setTextSize(2);
        // tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
        // tft.printf("Setpoint: %.2f", balanceSetpoint);
        // tft.print("         \n");
        // tft.printf("Angle: %.2f", imu.getEulerAngles().roll*RAD_2_DEG);
        // tft.print("         \n");
        // tft.printf("Control Effort: %.2f", balanceControlEffort);
        // tft.print("         \n");
        // tft.setCursor(0, 0);
        // tft.printf("%.2f     \n", balanceGains.kp);
        // tft.printf("%.2f     \n", balanceGains.ki);
        // tft.printf("%.2f     \n", balanceGains.kd);
        // tft.printf("%.2f     \n", balanceGains.trim);
        // tft.printf("\n%.2f     \n", balanceSetpoint);
        // tft.printf("%.2f     \n", imu.getEulerAngles().roll*RAD_2_DEG);
        // tft.printf("%.2f     \n", balanceControlEffort);
    }
}
//updates the PID parameters based on the potentiometer readings
//returns true if the PID parameters have changed
//take in desired PID parameters which are changed by the function
bool updatePIDParams(Gains &gains){
    //check the potentiometer readings
    bool newReading = readPots();
    if (newReading) {
        gains.kp = mapDouble(potMags[0], 0.0, 1.0, MIN_KP, MAX_KP);
        gains.ki = mapDouble(potMags[1], 0.0, 1.0, MIN_KI, MAX_KI);
        gains.kd = mapDouble(potMags[2], 0.0, 1.0, MIN_KD, MAX_KD);
        gains.trim = mapDouble(potMags[3], 0.0, 1.0, MIN_TRIM, MAX_TRIM);
    }
    return newReading;
}

//returns true if the angle is within the safe range
//This is useful for preventing the robot from tipping over
bool safeAngle(float angle, float trim, float maxSafeAngle){
    return (angle > -maxSafeAngle + trim) && (angle < maxSafeAngle + trim);
}