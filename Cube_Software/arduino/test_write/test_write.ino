#include <Wire.h>

void setup() {
    Wire.begin();
    
    Wire.beginTransmission(8);
    Wire.write(2);
    Wire.write(1);
    Wire.write(10);
    Wire.write(0);
    Wire.write(0);
    Wire.write(1);
    Wire.write(1);
    Wire.endTransmission();

    Wire.beginTransmission(9);
    Wire.write(2);
    Wire.write(1);
    Wire.write(10);
    Wire.write(0);
    Wire.write(0);
    Wire.write(1);
    Wire.write(1);
    Wire.endTransmission();

    Wire.beginTransmission(10);
    Wire.write(2);
    Wire.write(1);
    Wire.write(10);
    Wire.write(0);
    Wire.write(0);
    Wire.write(1);
    Wire.write(1);
    Wire.endTransmission();
}

void loop() {
}
