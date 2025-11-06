#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(9600);
}

uint8_t voxelType = 1;      // MATTER
uint8_t materialType = 2;   // PB_207_M
uint16_t activity = 0;  // 48 kBq
uint8_t isotopeSample = 0;  // NONE_S
uint8_t particlesDetectable = 0b00000110;  // GAMMA and BETA

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
    Wire.write(particlesDetectable);
    Wire.endTransmission();

    delay(5);

    Serial.println("Cube parameters written.");

    Serial.println("Requesting data...");
    Serial.println("\n");

    Wire.requestFrom(8, 7);  // Request 7 bytes

    Serial.print("VoxelType: ");
    Serial.println(Wire.read());
    Serial.print("MaterialType: ");
    Serial.println(Wire.read());
    Serial.print("Activity (Bq): ");
    Serial.println(Wire.read() | Wire.read() << 8);
    Serial.print("SampleType: ");
    Serial.println(Wire.read());

    Serial.println("Particles detectable: ");
    uint8_t partsDetectableRead = Wire.read()
    Serial.print("ALPHA: ");
    Serial.println((partsDetectableRead >> 0) & 1);
    Serial.print("BETA: ");
    Serial.println((partsDetectableRead >> 1) & 1);
    Serial.print("GAMMA: ");
    Serial.println((partsDetectableRead >> 2) & 1);
    Serial.print("NEUTRON: ");
    Serial.println((partsDetectableRead >> 3) & 1);

    Serial.print("Level: ");
    Serial.println(Wire.read());

    Serial.println("\n");
    if(!Wire.available()) {
        Serial.println("Finished reading data.");
    }

    delay(5000);
}
