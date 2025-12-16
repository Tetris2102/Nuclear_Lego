// data.cpp
// for some of the data, see https://www-nds.iaea.org

#include "../Material/material.h"
#include "../XSTable/xstable.h"
#include "../ParticleGroup/particleGroup.h"
#include "../IsotopeSample/isotopeSample.h"
#include <vector>

// Energy values used: 10 keV, 50 keV, 100 keV, 200 keV, 500 keV, 1 MeV, 2 MeV

// MUST maintain order of appending according to enums.h
inline std::vector<Material> getMaterials() {
    std::vector<Material> materials;

    materials.push_back(getM_Air());
    materials.push_back(getM_Wax());
    materials.push_back(getM_Pb207());
    materials.push_back(getM_Be9());
    materials.push_back(getM_U235());
    materials.push_back(getM_Pu239());

    return materials;
}

// MUST maintain order of appending according to enums.h
inline std::vector<IsotopeSample> getIsotopeSamples() {
    std::vector<IsotopeSample> samples;

    samples.push_back(getS_NULL());
    samples.push_back(getS_Ra226());
    samples.push_back(getS_Pu238());
    samples.push_back(getS_Sr90());
    samples.push_back(getS_Cs137());
    samples.push_back(getS_Cf252());

    return samples;
}

inline Material getM_Air() {
    // Air (atmospheric pressure, approx. 78% N2 + 21% O2)
    XSTable airTable;

    // Assuming no nuclear reactions occur in air (0.0 cross-section)
    airTable.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
    airTable.addRecordR(BETA, BETA, 1, 0.0, 0.0);
    airTable.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
    airTable.addRecordR(NEUTRON, NEUTRON, 1, 0.0, 0.0);

    airTable.addRecordS(ALPHA, 1000.0, 0.01);
    airTable.addRecordA(ALPHA, 100.0, 0.01);
    airTable.addRecordS(ALPHA, 200.0, 0.05);
    airTable.addRecordA(ALPHA, 30.0, 0.05);
    airTable.addRecordS(ALPHA, 80.0, 0.1);
    airTable.addRecordA(ALPHA, 10.0, 0.1);
    airTable.addRecordS(ALPHA, 30.0, 0.2);
    airTable.addRecordA(ALPHA, 5.0, 0.2);
    airTable.addRecordS(ALPHA, 10.0, 0.5);
    airTable.addRecordA(ALPHA, 2.0, 0.5);
    airTable.addRecordS(ALPHA, 5.0, 1.0);
    airTable.addRecordA(ALPHA, 1.0, 1.0);
    airTable.addRecordS(ALPHA, 2.0, 2.0);
    airTable.addRecordA(ALPHA, 0.5, 2.0);

    airTable.addRecordS(BETA, 100.0, 0.01);
    airTable.addRecordA(BETA, 20.0, 0.01);
    airTable.addRecordS(BETA, 30.0, 0.05);
    airTable.addRecordA(BETA, 5.0, 0.05);
    airTable.addRecordS(BETA, 10.0, 0.1);
    airTable.addRecordA(BETA, 2.0, 0.1);
    airTable.addRecordS(BETA, 4.0, 0.2);
    airTable.addRecordA(BETA, 1.0, 0.2);
    airTable.addRecordS(BETA, 1.0, 0.5);
    airTable.addRecordA(BETA, 0.3, 0.5);
    airTable.addRecordS(BETA, 0.5, 1.0);
    airTable.addRecordA(BETA, 0.1, 1.0);

    airTable.addRecordS(GAMMA, 0.4, 0.01);
    airTable.addRecordA(GAMMA, 0.2, 0.01);
    airTable.addRecordS(GAMMA, 0.2, 0.05);
    airTable.addRecordA(GAMMA, 0.1, 0.05);
    airTable.addRecordS(GAMMA, 0.15, 0.1);
    airTable.addRecordA(GAMMA, 0.08, 0.1);

    airTable.addRecordS(NEUTRON, 1.0, 0.01);
    airTable.addRecordA(NEUTRON, 0.05, 0.01);
    airTable.addRecordS(NEUTRON, 0.8, 0.05);
    airTable.addRecordA(NEUTRON, 0.03, 0.05);
    airTable.addRecordS(NEUTRON, 0.7, 0.1);
    airTable.addRecordA(NEUTRON, 0.02, 0.1);

    Material air("air", 29, 0.0012, airTable);
    return air;
}

inline Material getM_Wax() {
    XSTable waxTable;

    // add records to table

    return Material{"wax", 425, 0.88, waxTable};
}

// inline Material getM_Pb207() {
//     XSTable pb207Table;
//
//     // Need to add alpha and beta particle "absorption" emitting photon reaction
//     pb207Table.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
//     pb207Table.addRecordR(BETA, BETA, 1, 0.0, 0.0);
//     pb207Table.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
//     pb207Table.addRecordR(NEUTRON, NEUTRON, 1, 0.0, 0.0);
//
//     Material pb207("pb207", 207.0, 11.34, pb207Table);
//     return pb207;
// }

inline Material getM_Be9() {
    XSTable be9Table;

    // add records to table

    return Material{"Be9", 9, 1.85, be9Table};
}

inline Material getM_U235() {
    XSTable u235Table;

    // add records to table

    return Material{"U235", 235, 19.0, u235Table};
}

inline Material getM_Pu239() {
    XSTable pu239Table;

    // add records to table

    return Material{"Pu239", 239, 19.9, pu239Table};
}

inline IsotopeSample getS_NULL() {
    return IsotopeSample{};  // for this sample, name = "null"
}

inline IsotopeSample getS_Ra226() {
    return IsotopeSample{"Ra226 watch dial", ALPHA, 4.87, 20000.0};
}

inline IsotopeSample getS_Pu238() {
    return IsotopeSample{"Pu238 sample", ALPHA, 5.59, 50000.0};
}

inline IsotopeSample getS_Sr90() {
    return IsotopeSample{"Sr90 source", BETA, 0.546, 10000.0};
}

inline IsotopeSample getS_Cs137() {
    return IsotopeSample{"Cs137 lab sample", GAMMA, 0.662, 10000.0};
}

inline IsotopeSample getS_Cf252() {
    return IsotopeSample{"Cf252 lab sample", NEUTRON, 2.0, 1000.0};
}
