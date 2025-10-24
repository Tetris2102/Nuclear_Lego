#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(9600);
}

uint8_t voxelType = 1;      // MATTER
uint8_t materialType = 2;   // PB_207_M
uint16_t activity = 0;  // 48 kBq
uint8_t isotopeSample = 0;  // NONE_S

void loop() {
    Serial.println("Beginning communication...");

    // Least and most significant bytes of uint16_t activity
    uint8_t activityLeastByte = (uint8_t)(activity & 0xFF);
    uint8_t activityMostByte = (uint8_t)(activity >> 8);

    Wire.beginTransmission(8);  // Level 0 => address 8
    Wire.write(voxelType);
    Wire.write(materialType);
    Wire.write(activityLeastByte);
    Wire.write(activityMostByte);
    Wire.write(isotopeSample);
    Wire.endTransmission();

    delay(5);

    Serial.println("Cube parameters written.");

    Serial.println("Requesting data...");
    Serial.println("\n");

    Wire.requestFrom(8, 6);  // Request 6 bytes

    Serial.print("VoxelType: ");
    Serial.println(Wire.read());
    Serial.print("MaterialType: ");
    Serial.println(Wire.read());
    Serial.print("Activity (Bq): ");
    Serial.println(Wire.read() | Wire.read() << 8);
    Serial.print("SampleType: ");
    Serial.println(Wire.read());
    Serial.print("Level: ");
    Serial.println(Wire.read());

    Serial.println("\n");
    if(!Wire.available()) {
        Serial.println("Finished reading data.");
    }
}
