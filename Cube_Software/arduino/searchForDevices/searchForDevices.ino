#include <Wire.h>

void selectMuxChannel(uint8_t addr, uint8_t channel) {
    if(channel > 7) return;
    Wire.beginTransmission(addr);
    Wire.write(1 << channel);  // shift 1 left by channel bits
    Wire.endTransmission();
}

void deactivateMux(uint8_t addr) {
    Wire.beginTransmission(addr);
    Wire.write(0b00000000);
    Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);

  while(!Serial) {
    delay(10);
  }
}

bool detected = false;

void loop() {
  Serial.println("Searching for devices...");

  selectMuxChannel(0x71, 0);

  for(uint8_t i = 0; i<127; i++) {
      Wire.beginTransmission(i);
      uint8_t error = Wire.endTransmission();

      if(error == 0) {
        detected = true;
        Serial.print("Device detected at 0x");
        if(i<16) Serial.print("0");
        Serial.println(i, HEX);
      }
  }

  if(!detected) Serial.println("No devices detected!");
  Serial.println("\n");

  delay(5000);
}
