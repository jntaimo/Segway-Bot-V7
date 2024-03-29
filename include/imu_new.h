#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include "EulerAngles.h"
#include "EveryNMillis.h"
#include "pinout.h"

extern EulerAngles angles;
extern GyroReadings gyro;

extern volatile bool imuDataReady;

void imuSetup();
void setReports();
void readIMU();
bool updateIMU();
void imuISR();
void printEuler(EulerAngles angles);