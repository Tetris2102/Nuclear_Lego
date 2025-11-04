#include <TinyWireS.h>
#include <EEPROM.h>

#define MAX_LEVEL 2  // Maximum height MINUS 1, so indexing begins from 0

// SDA at PB0
// SCL at PB2
#define PIN_BUZZ 3     // Buzzer pin
// Have to connect RC network to PIN_LVL_OUT
// to smooth PWM (C=4.7uF, R=1.0kOhm)
#define PIN_LVL_OUT 1
#define PIN_LVL_IN 4

// Corresponding enums are stored in main code (enums.h)
uint8_t voxelType;           // at EEPROM 0
uint8_t materialType;        // at EEPROM 1
uint16_t activity;           // no need to store
uint8_t sampleType;          // at EEPROM 2
uint8_t particlesDetectable  // at EEPROM 3
// particlesDetectable represents [NEUTRON, GAMMA, BETA, ALPHA]
// (encoded into the 4 least significant bits)

uint8_t level;

void updateParams();
void reportOnRequest();
void receiveAndStore(uint8_t numBytes);
uint16_t msBetweenDecays(uint16_t activity);
void beepBuzzer();

void setup() {
    pinMode(PIN_BUZZ, OUTPUT);
    pinMode(PIN_LVL_OUT, OUTPUT);
    pinMode(PIN_LVL_IN, INPUT);

    randomSeed(analogRead(PIN_LVL_IN));

    updateParams();

    TinyWireS.begin(8 + level);  // Use level as I2C address beginning from 8
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
    // Get parameters from EEPROM on startup
    voxelType = EEPROM.get(0);
    materialType = EEPROM.get(1);
    sampleType = EEPROM.get(2);
    particlesDetectable = EEPROM.get(3);

    // Determine voxel's level
    uint16_t v_in = analogRead(PIN_LVL_IN);
    if(v_in > 299) {         // threshold at 577 mV
        level = 0;
    } else if(v_in > 118) {  // threshold at 1.462 V
        level = 1;
    } else {
        level = 2;
    }
}

void reportOnRequest() {
    TinyWireS.write(voxelType);
    TinyWireS.write(materialType);
    TinyWireS.write(activity & 0xFF);  // Least significant byte first
    TinyWireS.write(activity >> 8);    // Most significant byte then
    TinyWireS.write(sampleType);
    TinyWireS.write(particlesDetectable);
    TinyWireS.write(level);
}

void receiveAndStore(uint8_t numBytes) {
    if(numBytes == 5) {
        voxelType = TinyWireS.read();
        materialType = TinyWireS.read();
        activity = TinyWireS.read() | (TinyWireS.read() << 8);
        sampleType = TinyWireS.read();
        particlesDetectable = TinyWireS.read();

        EEPROM.update(0, voxelType);
        EEPROM.update(1, materialType);
        EEPROM.update(2, sampleType);
        EEPROM.update(3, particlesDetectable);
    }
}

uint16_t msBetweenDecays(uint16_t activity) {
    if(activity > 500) activity = 500;  // 500 decays/second max
    // Handle background radiation in case activity is small
    if(activity == 0) activity = 1;

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
