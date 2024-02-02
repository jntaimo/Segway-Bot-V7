#include <Arduino.h>
#include <ESP32Encoder.h>
#include "pinout.h"

ESP32Encoder encoder;


void setup(){
	
	Serial.begin();

	// Enable the weak pull down resistors
	ESP32Encoder::useInternalWeakPullResistors=UP;

	encoder.attachFullQuad(LEFT_ENCODER_A_PIN, LEFT_ENCODER_B_PIN);
}

void loop(){
	// Loop and read the count
	Serial.println("Encoder count = " + String((int32_t)encoder.getCount()));
	delay(100);
}