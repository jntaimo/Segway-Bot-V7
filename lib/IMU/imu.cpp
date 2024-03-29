#include "imu.h"


IMU::IMU(int resetPin, int csPin, int intPin) 
: bno08x(resetPin), imuDataReady(false), _resetPin(resetPin), _csPin(csPin), _intPin(intPin){

}

void IMU::imuISR() {
    imu.imuDataReady = true;
}

void IMU::setup() {
    if (IMU_DEBUG) Serial.println("Setting up IMU");

    if (!bno08x.begin_SPI(_csPin, _intPin)) {
        if (IMU_DEBUG) Serial.println("Failed to find BNO08x chip");
        while (1) { delay(10); }
    } 

    if (IMU_DEBUG) Serial.println("BNO08x Found!");

    setReports();
    pinMode(_intPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(_intPin), imuISR, RISING);
}

void IMU::setReports() {
    if (IMU_DEBUG) Serial.println("Setting desired reports");
    if (! bno08x.enableReport(SH2_GAME_ROTATION_VECTOR, 2500)) {
        if (IMU_DEBUG) Serial.println("Could not enable game vector");
    }
    if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED, 1500)) {
        if (IMU_DEBUG) Serial.println("Could not enable gyroscope");
    }
}
//Actually read the values on the IMU
//This should only be called when the IMU data is ready
void IMU::readIMU() {
    if (bno08x.wasReset()) {
        if (IMU_DEBUG) Serial.print("sensor was reset ");
        setReports();
        return;
    }

    if (! bno08x.getSensorEvent(&sensorValue)) {
        return;
    }

    switch (sensorValue.sensorId) {
        case SH2_GAME_ROTATION_VECTOR:
            Quaternion quatReading;
            quatReading.w = sensorValue.un.gameRotationVector.real;
            quatReading.x = sensorValue.un.gameRotationVector.i;
            quatReading.y = sensorValue.un.gameRotationVector.j;
            quatReading.z = sensorValue.un.gameRotationVector.k;

            eulerAngles =  ToEulerAngles(quatReading);
        break;

        case SH2_GYROSCOPE_CALIBRATED:
            gyroReadings.rollRate = sensorValue.un.gyroscope.x;
            gyroReadings.pitchRate = sensorValue.un.gyroscope.y;
            gyroReadings.yawRate= sensorValue.un.gyroscope.z;
        break;
    }  
}

void IMU::update() {
    if (imuDataReady) {
        readIMU();
        imuDataReady = false;
    }
}

GyroReadings IMU::getGyroReadings() {
    return gyroReadings;
}

EulerAngles IMU::getEulerAngles() {
    return eulerAngles;
}
