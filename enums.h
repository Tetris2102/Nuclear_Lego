// enums.h

#ifndef ENUMS_H
#define ENUMS_H
#include <cstdint>

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
    PU_239_M
};

enum IsotopeSampleType : uint8_t {
    NULL_S,
    RA_226_S,  // alpha
    PU_238_S,  // alpha
    SR_90_S,   // beta
    CS_137_S,  // gamma
    CF_252_S   // neutron
};

#endif
