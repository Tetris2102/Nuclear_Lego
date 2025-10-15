#include <TinyWireS.h>
#include <EEPROM.h>
#include "enums.h"

#define MAX_HEIGHT 2  // Maximum height MINUS 1, so indexing begins from 0
#define PWM_FREQ_STEP 100  //  100 Hz step

#define PIN_SDA 0
#define PIN_SCL 2
#define PIN_PRES_IN 1  // Upper presence pin
#define PIN_BUZZ 3     // Buzzer pin
#define PIN_LVL_OUT 4
#define PIN_LVL_IN 5

VoxelType voxelType;           // at EEPROM 0
MaterialType materialType;     // at EEPROM 1
SourceActivity activity;       // at EEPROM 2
IsotopeSampleType sampleType;  // at EEPROM 3

uint8_t level;
bool hasCubeAbove;

void updateParams();
void reportOnRequest();
void receiveAndStore(uint8_t numBytes);

void setup() {
    pinMode(PIN_PRES_IN, INPUT);
    pinMode(PIN_BUZZ, OUTPUT);
    pinMode(PIN_LVL_OUT, OUTPUT);
    pinMode(PIN_LVL_IN, INPUT);

    updateParams();

    TinyWireS.begin(level + 8);  // Use level as I2C address beginning from 8
    // Safe I2C addresses (not reserved): 8-119

    TinyWireS.onRequest(reportOnRequest);
    TinyWireS.onReceive(receiveAndStore);
}

void loop() {}

void updateParams() {
    uint16_t lvl_in = analogRead(PIN_LVL_IN);
    level = map(lvl_in, 0, 65535, 0, MAX_HEIGHT);

    if(lvl_in != MAX_HEIGHT) {
        uint8_t lvl_out = map(level + 1, 0, 65535, 0, MAX_HEIGHT);
        analogWrite(PIN_LVL_OUT, lvl_out);
    } else {
        digitalWrite(PIN_LVL_OUT, HIGH);
    }

    hasCubeAbove = digitalRead(PIN_PRES_IN);
}

void reportOnRequest() {
    TinyWireS.write(static_cast<uint8_t>(voxelType));
    TinyWireS.write(static_cast<uint8_t>(materialType));
    TinyWireS.write(static_cast<uint8_t>(activity));
    TinyWireS.write(static_cast<uint8_t>(sampleType));
    TinyWireS.write(level);
    TinyWireS.write(hasCubeAbove);
}

void receiveAndStore(uint8_t numBytes) {
    if(numBytes >= 2) {
        uint8_t enumID = TinyWireS.read();
        uint8_t enumVal = TinyWireS.read();

        switch(enumID) {
            case 0: voxelType = static_cast<VoxelType>(enumVal); break;
            case 1: materialType = static_cast<MaterialType>(enumVal); break;
            case 2: activity = static_cast<SourceActivity>(enumVal); break;
            case 3: sampleType = static_cast<IsotopeSampleType>(enumVal); break;
        }

        EEPROM.update(enumID, enumVal);
    }
}
