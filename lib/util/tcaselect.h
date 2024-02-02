#ifndef TCA_SELECT_H
#define TCA_SELECT_H

#include <Wire.h>
#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 8) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i-1);
  Wire.endTransmission();  
}

#endif