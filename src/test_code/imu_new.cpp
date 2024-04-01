#include "imu_new.h"
#include "display.h"

#define PRINT_INTERVAL 1
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);



Adafruit_BNO08x  bno08x(IMU_RST);
sh2_SensorValue_t sensorValue;
EulerAngles angles;
GyroReadings gyro;

volatile bool imuDataReady = false;

void imuISR() {
  imuDataReady = true;
}

void imuSetup(void) {

  Serial.println("Setting up IMU");

  if (!bno08x.begin_SPI(IMU_CS, IMU_INT)) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  } 

  Serial.println("BNO08x Found!");

  setReports();
  pinMode(IMU_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(IMU_INT), imuISR, RISING);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(SH2_GAME_ROTATION_VECTOR, 1000)) {
    Serial.println("Could not enable game vector");
  }
  // if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED, 1000)) {
  //   Serial.println("Could not enable gyroscope");
  // }
}

//returns the current euler angles in radians
//EulerAngles.success returns whether a reading was successfully acquired
void readIMU() {

    //if something sus happened to the IMU, reset it
    if (bno08x.wasReset()) {
        Serial.print("sensor was reset ");
        setReports();
    }
    
    //if reading failed
    if (! bno08x.getSensorEvent(&sensorValue)) {
        return;
    }
    
    //determine which sensor reading was received and store it in the approprite variable
    switch (sensorValue.sensorId) {
        //if it was an euler angle reading
        case SH2_GAME_ROTATION_VECTOR:
        //store new reading in a quaternion
            Quaternion quatReading;
            quatReading.w = sensorValue.un.gameRotationVector.real;
            quatReading.x = sensorValue.un.gameRotationVector.i;
            quatReading.y = sensorValue.un.gameRotationVector.j;
            quatReading.z = sensorValue.un.gameRotationVector.k;

            //store the current angles
            angles =  ToEulerAngles(quatReading);
            break;
        //if it was a gyro reading
        case SH2_GYROSCOPE_CALIBRATED:
          //store the current gyro readings
          gyro.rollRate = sensorValue.un.gyroscope.x;
          gyro.pitchRate = sensorValue.un.gyroscope.y;
          gyro.yawRate = sensorValue.un.gyroscope.z;
          break;
          

    }

}



bool updateIMU() {
  if (imuDataReady) {
    readIMU();
    imuDataReady = false;
    return true;
  }
  return false;
}

void setup(){
  Serial.begin(115200);
  imuSetup();
  displaySetup();
  pinMode(TFT_CS, OUTPUT);
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(2);
}

unsigned long lastPrint = 0;
int printTime = 0;

double derivative = 0;
double lastAngle = 0;
void loop(){
  //update the IMU every loop 
  updateIMU();
  
  EVERY_N_MILLIS(PRINT_INTERVAL){
    derivative = 1000*(angles.roll - lastAngle)/printTime;
    //readIMU();
    tft.setCursor(0, 0);
    // tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    // tft.println("Euler Angles (deg):");
    // tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
    tft.printf("%.2f    \n", angles.roll*RAD_2_DEG);
    tft.printf("%.2f   \n", derivative);
    tft.printf("%d   \n", printTime);
    printTime = millis()-lastPrint;
    lastPrint = millis();
    lastAngle = angles.roll;

  }
  
  EVERY_N_MILLIS(100){
    
    
    //Serial.printf("Roll: %.1f printTime (ms) %d\n", angles.roll*RAD_2_DEG, printTime);
    // Serial.printf("RollRate: %.2f PitchRate: %.2f YawRate: %.2f\n", gyro.rollRate*RAD_2_DEG, gyro.pitchRate*RAD_2_DEG, gyro.yawRate*RAD_2_DEG);
  }
}
