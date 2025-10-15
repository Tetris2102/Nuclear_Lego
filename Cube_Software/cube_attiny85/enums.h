// enums.h

#ifndef ENUMS_H
#define ENUMS_H

enum VoxelType : uint8_t {
    SOURCE,
    MATTER,
    DETECTOR
};

enum MaterialType : uint8_t {
    AIR_M,
    WAX_M,
    PB_207_M,
    BE_9_M,
    U_235_M,
    PU_238_M
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
    NONE_S,
    RA_226_S,  // alpha
    PU_238_S,  // alpha
    SR_90_S,   // beta
    CS_137_S,  // gamma
    CF_252_S   // neutron
};

#endif
