#ifndef EULER_ANGLES_H
#define EULER_ANGLES_H
#include <Arduino.h>
#define RAD_2_DEG 57.295779513082320876798154814105
#pragma DEG_2_RAD 0.0174532925199432957692369076848
struct Quaternion {
    double w, x, y, z;
};

struct EulerAngles {
    double roll, pitch, yaw;
};

struct GyroReadings {
    double rollRate, pitchRate, yawRate;
};
EulerAngles ToEulerAngles(Quaternion q);
void printEuler(EulerAngles angles);
void printEulerDeg(EulerAngles angles);
void printGyro(GyroReadings gyroReadings);
void printGyroDeg(GyroReadings gyroReadings);
#endif // EULER_ANGLES_H