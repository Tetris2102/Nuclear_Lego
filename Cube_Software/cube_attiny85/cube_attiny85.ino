#include <TinyWireS.h>
#include <EEPROM.h>
#include "enums.h"

#define MAX_HEIGHT 2  // Maximum height MINUS 1, so indexing begins from 0

// SDA at PB0
// SCL at PB2
#define PIN_PRES_IN 1  // Upper presence pin
#define PIN_BUZZ 3     // Buzzer pin
#define PIN_LVL_OUT 4
#define PIN_LVL_IN 5

VoxelType voxelType;           // at EEPROM 0
MaterialType materialType;     // at EEPROM 1
uint16_t activity;       // at EEPROM 2 and 3
IsotopeSampleType sampleType;  // at EEPROM 4

uint8_t level;
bool hasCubeAbove;

void updateParams();
void reportOnRequest();
void receiveAndStore(uint8_t numBytes);
uint16_t msBetweenDecays(uint16_t activity);
void beepBuzzer();

void setup() {
    pinMode(PIN_PRES_IN, INPUT);
    pinMode(PIN_BUZZ, OUTPUT);
    pinMode(PIN_LVL_OUT, OUTPUT);
    pinMode(PIN_LVL_IN, INPUT);

    randomSeed(analogRead(PIN_LVL_IN));

    updateParams();

    TinyWireS.begin(level + 8);  // Use level as I2C address beginning from 8
    // Safe I2C addresses (not reserved): 8-119

    TinyWireS.onRequest(reportOnRequest);
    TinyWireS.onReceive(receiveAndStore);
}

uint16_t decay_interval_ms;
unsigned long last_beep;

void loop() {
    TinyWireS_stop_check();

    unsigned long now = millis();

    if(now - last_beep > decay_interval_ms) {
        beepBuzzer();
        decay_interval_ms = msBetweenDecays(activity);
        last_beep = now;
    }
}

void updateParams() {
    uint16_t lvl_in = analogRead(PIN_LVL_IN);
    level = map(lvl_in, 0, 1023, 0, MAX_HEIGHT);

    if(level != MAX_HEIGHT) {
        uint8_t lvl_out = map(level + 1, 0, MAX_HEIGHT, 0, 255);
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
    if(numBytes == 6) {
        voxelType = static_cast<VoxelType>(TinyWireS.read());
        materialType = static_cast<MaterialType>(TinyWireS.read());
        activity = TinyWireS.read() | (TinyWireS.read() << 8);
        sampleType = static_cast<IsotopeSampleType>(TinyWireS.read());

        EEPROM.update(0, voxelType);
        EEPROM.update(1, materialType);
        EEPROM.update(2, activity & 0xFF);
        EEPROM.update(3, activity >> 8);
        EEPROM.update(4, sampleType);
    }
}

uint16_t msBetweenDecays(uint16_t activity) {
    if(activity > 2000) activity = 2000;
    if(activity == 0) activity = 1;  // Handle background radiation
                                     // in case activity is small

    // Poisson time scaling with random number generator
    uint16_t r = random(1, 65535);

    uint32_t U = r;
    uint32_t diff = 65535 - U;
    uint32_t ln_approx = diff + (diff * diff) / (2 * 65535) +
      (diff * diff * diff) / (3 * 65535 * 65535);

    uint16_t mean_ms = 1000UL / activity;
    uint32_t result = (mean_ms * ln_approx) / 65535;

    return (uint16_t)result;
}

void beepBuzzer() {
    for(uint8_t i = 0; i<20; i++) {
        digitalWrite(PIN_BUZZ, HIGH);
        delayMicroseconds(125);
        digitalWrite(PIN_BUZZ, LOW);
        delayMicroseconds(125);
    }
}
