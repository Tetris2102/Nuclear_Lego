#include <Wire.h>

#define MUX1_ADDR 0x71
#define MUX2_ADDR 0x70

uint8_t voxelType = 1;      // MATTER
uint8_t materialType = 2;   // PB_207_M
uint16_t activity = 100;  // 48 kBq
uint8_t isotopeSample = 0;  // NONE_S
uint8_t particlesDetectable = 0b00000000;  // GAMMA and BETA

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
    Wire.begin();
    Wire.setClock(100000);
    Serial.begin(9600);

    Serial.println("Beginning communication...");

    for(uint8_t i = 8; i<11; i++) {
        scanMux1Channel(i);
    }
}

uint8_t error;

void loop() {
}

void scanMux1Channel(uint8_t addr) {
      // Least and most significant bytes of uint16_t activity
      uint8_t activityLeastByte = (uint8_t)(activity & 0xFF);
      uint8_t activityMostByte = (uint8_t)(activity >> 8);

      selectMuxChannel(MUX1_ADDR, 0);
      Wire.beginTransmission(addr);  // Level 0 => address 8
      Wire.write(voxelType);
      Wire.write(materialType);
      Wire.write(activityLeastByte);
      Wire.write(activityMostByte);
      Wire.write(isotopeSample);
      Wire.write(particlesDetectable);
      error = Wire.endTransmission();

      if(error == 0) {
          Serial.println("Successful write to cube on MUX1");
      } else {
          Serial.println("Failed to write to cube at MUX1");
      }
      deactivateMux(MUX1_ADDR);

      delay(5);

      Serial.println("Cube parameters written.");

      Serial.println("Requesting data...");
      Serial.println("\n");

      selectMuxChannel(MUX1_ADDR, 0);
      Wire.requestFrom(addr, 7);  // Request 7 bytes

      Serial.print("VoxelType: ");
      Serial.println(Wire.read());
      Serial.print("MaterialType: ");
      Serial.println(Wire.read());
      Serial.print("Activity (Bq): ");
      Serial.println(Wire.read() | Wire.read() << 8);
      Serial.print("SampleType: ");
      Serial.println(Wire.read());

      Serial.println("ParticleGroups detectable: ");
      uint8_t partsDetectableRead = Wire.read();
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
      deactivateMux(MUX1_ADDR);

      Serial.println("\n");
      if(!Wire.available()) {
          Serial.println("Finished reading data.");
      } else {
          Serial.println("Data reading not finished.");
      }
}
