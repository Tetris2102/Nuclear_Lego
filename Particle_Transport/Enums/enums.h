#ifndef ENUMS_H
#define ENUMS_H

enum VoxelType : uint8_t {
    SOURCE,
    MATTER,
    DETECTOR
};

enum MaterialType : uint8_t {
    AIR,
    WAX,
    PB_207,
    BE_9,
    U_235,
    PU_238
};

// In Bequerels (decays/second)
enum SourceActivity : uint8_t {
    _0,
    _50,
    _100,
    _500,
    _1000,
    _5000,
    _10000,
    _50000
};

enum IsotopeSampleType : uint8_t {
    RA_226,  // alpha
    PU_238,  // alpha
    SR_90,   // beta
    CS_137,  // gamma
    CF_252   // neutron
}

#endif
