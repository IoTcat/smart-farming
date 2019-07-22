#include<Wire.h>
#include"GY30.h"
#include <Arduino.h>

int GY30::getLight()
{
  // put your main code here, to run repeatedly:
  int val = 0;
  // reset
  Wire.beginTransmission(ADDR);
  Wire.write(0b00000111);
  Wire.endTransmission();
 
  Wire.beginTransmission(ADDR);
  Wire.write(0b00100000);
  Wire.endTransmission();
  // typical read delay 120ms
  delay(120);
  Wire.requestFrom(ADDR, 2); // 2byte every time
  for (val = 0; Wire.available() >= 1; ) {
    char c = Wire.read();
    //Serial.println(c, HEX);
    val = (val << 8) + (c & 0xFF);
  }
  val = val / 1.2;
  return val;
}

