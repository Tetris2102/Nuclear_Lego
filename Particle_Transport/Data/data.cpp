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
    // Paraffin wax (C25H52) - hydrogen-rich neutron moderator
    // Effective atomic mass ~14 (average of C and H weighted)
    XSTable waxTable;

    // Reaction records (no nuclear reactions for charged particles)
    waxTable.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
    waxTable.addRecordR(BETA, BETA, 1, 0.0, 0.0);
    waxTable.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
    waxTable.addRecordR(NEUTRON, NEUTRON, 1, 0.0, 0.0);

    // Alpha particles - high stopping power in hydrogenous material
    waxTable.addRecordS(ALPHA, 1500.0, 0.01);
    waxTable.addRecordA(ALPHA, 150.0, 0.01);
    waxTable.addRecordS(ALPHA, 300.0, 0.05);
    waxTable.addRecordA(ALPHA, 50.0, 0.05);
    waxTable.addRecordS(ALPHA, 120.0, 0.1);
    waxTable.addRecordA(ALPHA, 20.0, 0.1);
    waxTable.addRecordS(ALPHA, 50.0, 0.2);
    waxTable.addRecordA(ALPHA, 8.0, 0.2);
    waxTable.addRecordS(ALPHA, 15.0, 0.5);
    waxTable.addRecordA(ALPHA, 3.0, 0.5);
    waxTable.addRecordS(ALPHA, 8.0, 1.0);
    waxTable.addRecordA(ALPHA, 1.5, 1.0);
    waxTable.addRecordS(ALPHA, 3.0, 2.0);
    waxTable.addRecordA(ALPHA, 0.8, 2.0);

    // Beta particles - moderate stopping power
    waxTable.addRecordS(BETA, 150.0, 0.01);
    waxTable.addRecordA(BETA, 30.0, 0.01);
    waxTable.addRecordS(BETA, 50.0, 0.05);
    waxTable.addRecordA(BETA, 8.0, 0.05);
    waxTable.addRecordS(BETA, 15.0, 0.1);
    waxTable.addRecordA(BETA, 3.0, 0.1);
    waxTable.addRecordS(BETA, 6.0, 0.2);
    waxTable.addRecordA(BETA, 1.5, 0.2);
    waxTable.addRecordS(BETA, 2.0, 0.5);
    waxTable.addRecordA(BETA, 0.5, 0.5);
    waxTable.addRecordS(BETA, 0.8, 1.0);
    waxTable.addRecordA(BETA, 0.2, 1.0);

    // Gamma - low absorption (low Z material)
    waxTable.addRecordS(GAMMA, 0.3, 0.01);
    waxTable.addRecordA(GAMMA, 0.1, 0.01);
    waxTable.addRecordS(GAMMA, 0.15, 0.05);
    waxTable.addRecordA(GAMMA, 0.05, 0.05);
    waxTable.addRecordS(GAMMA, 0.1, 0.1);
    waxTable.addRecordA(GAMMA, 0.03, 0.1);

    // Neutrons - excellent moderator due to hydrogen content
    // High elastic scattering cross-section from hydrogen (~20 barns)
    waxTable.addRecordS(NEUTRON, 25.0, 0.01);
    waxTable.addRecordA(NEUTRON, 0.3, 0.01);
    waxTable.addRecordS(NEUTRON, 22.0, 0.05);
    waxTable.addRecordA(NEUTRON, 0.15, 0.05);
    waxTable.addRecordS(NEUTRON, 20.0, 0.1);
    waxTable.addRecordA(NEUTRON, 0.08, 0.1);
    waxTable.addRecordS(NEUTRON, 18.0, 0.5);
    waxTable.addRecordA(NEUTRON, 0.02, 0.5);
    waxTable.addRecordS(NEUTRON, 15.0, 1.0);
    waxTable.addRecordA(NEUTRON, 0.01, 1.0);
    waxTable.addRecordS(NEUTRON, 10.0, 2.0);
    waxTable.addRecordA(NEUTRON, 0.005, 2.0);

    return Material{"wax", 14, 0.88, waxTable};
}

inline Material getM_Pb207() {
    // Lead-207 - excellent gamma shielding, high Z material
    XSTable pb207Table;

    // Reaction records
    pb207Table.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
    pb207Table.addRecordR(BETA, BETA, 1, 0.0, 0.0);
    pb207Table.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
    pb207Table.addRecordR(NEUTRON, NEUTRON, 1, 0.0, 0.0);

    // Alpha particles - very high stopping power (high Z)
    pb207Table.addRecordS(ALPHA, 5000.0, 0.01);
    pb207Table.addRecordA(ALPHA, 500.0, 0.01);
    pb207Table.addRecordS(ALPHA, 1000.0, 0.05);
    pb207Table.addRecordA(ALPHA, 150.0, 0.05);
    pb207Table.addRecordS(ALPHA, 400.0, 0.1);
    pb207Table.addRecordA(ALPHA, 60.0, 0.1);
    pb207Table.addRecordS(ALPHA, 150.0, 0.2);
    pb207Table.addRecordA(ALPHA, 25.0, 0.2);
    pb207Table.addRecordS(ALPHA, 50.0, 0.5);
    pb207Table.addRecordA(ALPHA, 10.0, 0.5);
    pb207Table.addRecordS(ALPHA, 25.0, 1.0);
    pb207Table.addRecordA(ALPHA, 5.0, 1.0);
    pb207Table.addRecordS(ALPHA, 10.0, 2.0);
    pb207Table.addRecordA(ALPHA, 2.0, 2.0);

    // Beta particles - high stopping power
    pb207Table.addRecordS(BETA, 500.0, 0.01);
    pb207Table.addRecordA(BETA, 100.0, 0.01);
    pb207Table.addRecordS(BETA, 150.0, 0.05);
    pb207Table.addRecordA(BETA, 30.0, 0.05);
    pb207Table.addRecordS(BETA, 50.0, 0.1);
    pb207Table.addRecordA(BETA, 10.0, 0.1);
    pb207Table.addRecordS(BETA, 20.0, 0.2);
    pb207Table.addRecordA(BETA, 5.0, 0.2);
    pb207Table.addRecordS(BETA, 8.0, 0.5);
    pb207Table.addRecordA(BETA, 2.0, 0.5);
    pb207Table.addRecordS(BETA, 4.0, 1.0);
    pb207Table.addRecordA(BETA, 1.0, 1.0);

    // Gamma - HIGH absorption (photoelectric effect dominant at low energies)
    // Pb has Z=82, excellent gamma shielding
    pb207Table.addRecordS(GAMMA, 5.0, 0.01);
    pb207Table.addRecordA(GAMMA, 150.0, 0.01);  // High photoelectric absorption
    pb207Table.addRecordS(GAMMA, 8.0, 0.05);
    pb207Table.addRecordA(GAMMA, 20.0, 0.05);
    pb207Table.addRecordS(GAMMA, 10.0, 0.1);
    pb207Table.addRecordA(GAMMA, 5.0, 0.1);
    pb207Table.addRecordS(GAMMA, 8.0, 0.2);
    pb207Table.addRecordA(GAMMA, 2.0, 0.2);
    pb207Table.addRecordS(GAMMA, 5.0, 0.5);
    pb207Table.addRecordA(GAMMA, 0.8, 0.5);
    pb207Table.addRecordS(GAMMA, 3.0, 1.0);
    pb207Table.addRecordA(GAMMA, 0.5, 1.0);
    pb207Table.addRecordS(GAMMA, 2.0, 2.0);
    pb207Table.addRecordA(GAMMA, 0.3, 2.0);

    // Neutrons - moderate scattering, low absorption
    // Total XS ~7-11 barns
    pb207Table.addRecordS(NEUTRON, 11.0, 0.01);
    pb207Table.addRecordA(NEUTRON, 0.03, 0.01);
    pb207Table.addRecordS(NEUTRON, 10.0, 0.05);
    pb207Table.addRecordA(NEUTRON, 0.02, 0.05);
    pb207Table.addRecordS(NEUTRON, 9.0, 0.1);
    pb207Table.addRecordA(NEUTRON, 0.01, 0.1);
    pb207Table.addRecordS(NEUTRON, 8.0, 0.5);
    pb207Table.addRecordA(NEUTRON, 0.005, 0.5);
    pb207Table.addRecordS(NEUTRON, 7.0, 1.0);
    pb207Table.addRecordA(NEUTRON, 0.003, 1.0);
    pb207Table.addRecordS(NEUTRON, 6.0, 2.0);
    pb207Table.addRecordA(NEUTRON, 0.002, 2.0);

    Material pb207("Pb207", 207, 11.34, pb207Table);
    return pb207;
}

inline Material getM_Be9() {
    // Beryllium-9 - neutron reflector/multiplier
    // Important (n,2n) reaction above ~1.85 MeV threshold
    // Also has (α,n) reaction used in neutron sources
    XSTable be9Table;

    // Reaction records
    be9Table.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
    be9Table.addRecordR(BETA, BETA, 1, 0.0, 0.0);
    be9Table.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
    // (n,2n) reaction: 9Be + n → 2α + 2n (threshold ~1.85 MeV)
    be9Table.addRecordR(NEUTRON, NEUTRON, 2, 0.5, 2.0);  // ~0.5 barn at 2 MeV
    // (α,n) reaction: 9Be + α → 12C + n (used in AmBe, PuBe sources)
    be9Table.addRecordR(ALPHA, NEUTRON, 1, 0.4, 5.0);  // ~0.4 barn at 5 MeV alpha

    // Alpha particles - moderate stopping power (low Z)
    be9Table.addRecordS(ALPHA, 800.0, 0.01);
    be9Table.addRecordA(ALPHA, 80.0, 0.01);
    be9Table.addRecordS(ALPHA, 200.0, 0.05);
    be9Table.addRecordA(ALPHA, 30.0, 0.05);
    be9Table.addRecordS(ALPHA, 80.0, 0.1);
    be9Table.addRecordA(ALPHA, 12.0, 0.1);
    be9Table.addRecordS(ALPHA, 35.0, 0.2);
    be9Table.addRecordA(ALPHA, 5.0, 0.2);
    be9Table.addRecordS(ALPHA, 12.0, 0.5);
    be9Table.addRecordA(ALPHA, 2.0, 0.5);
    be9Table.addRecordS(ALPHA, 6.0, 1.0);
    be9Table.addRecordA(ALPHA, 1.0, 1.0);
    be9Table.addRecordS(ALPHA, 2.5, 2.0);
    be9Table.addRecordA(ALPHA, 0.5, 2.0);

    // Beta particles - low stopping power (low Z)
    be9Table.addRecordS(BETA, 80.0, 0.01);
    be9Table.addRecordA(BETA, 15.0, 0.01);
    be9Table.addRecordS(BETA, 25.0, 0.05);
    be9Table.addRecordA(BETA, 5.0, 0.05);
    be9Table.addRecordS(BETA, 10.0, 0.1);
    be9Table.addRecordA(BETA, 2.0, 0.1);
    be9Table.addRecordS(BETA, 4.0, 0.2);
    be9Table.addRecordA(BETA, 0.8, 0.2);
    be9Table.addRecordS(BETA, 1.5, 0.5);
    be9Table.addRecordA(BETA, 0.3, 0.5);
    be9Table.addRecordS(BETA, 0.6, 1.0);
    be9Table.addRecordA(BETA, 0.1, 1.0);

    // Gamma - low absorption (low Z)
    be9Table.addRecordS(GAMMA, 0.25, 0.01);
    be9Table.addRecordA(GAMMA, 0.08, 0.01);
    be9Table.addRecordS(GAMMA, 0.12, 0.05);
    be9Table.addRecordA(GAMMA, 0.04, 0.05);
    be9Table.addRecordS(GAMMA, 0.08, 0.1);
    be9Table.addRecordA(GAMMA, 0.02, 0.1);

    // Neutrons - good scatterer, very low absorption
    // Total XS ~6 barns, absorption ~0.0076 barns (thermal)
    be9Table.addRecordS(NEUTRON, 6.5, 0.01);
    be9Table.addRecordA(NEUTRON, 0.008, 0.01);
    be9Table.addRecordS(NEUTRON, 6.2, 0.05);
    be9Table.addRecordA(NEUTRON, 0.004, 0.05);
    be9Table.addRecordS(NEUTRON, 6.0, 0.1);
    be9Table.addRecordA(NEUTRON, 0.003, 0.1);
    be9Table.addRecordS(NEUTRON, 5.5, 0.5);
    be9Table.addRecordA(NEUTRON, 0.002, 0.5);
    be9Table.addRecordS(NEUTRON, 5.0, 1.0);
    be9Table.addRecordA(NEUTRON, 0.001, 1.0);
    be9Table.addRecordS(NEUTRON, 4.0, 2.0);
    be9Table.addRecordA(NEUTRON, 0.0008, 2.0);

    return Material{"Be9", 9, 1.85, be9Table};
}

inline Material getM_U235() {
    // Uranium-235 - fissile material
    // High fission cross-section for thermal/epithermal neutrons
    // Density of enriched uranium metal ~19.0 g/cm³
    XSTable u235Table;

    // Reaction records
    u235Table.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
    u235Table.addRecordR(BETA, BETA, 1, 0.0, 0.0);
    u235Table.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
    // Fission: n + U235 → fission products + ~2.5 neutrons (avg)
    // Thermal fission XS: ~585 barns, decreases with energy
    u235Table.addRecordR(NEUTRON, NEUTRON, 3, 585.0, 0.025);   // Thermal (~0.025 eV)
    u235Table.addRecordR(NEUTRON, NEUTRON, 3, 50.0, 0.01);     // 10 keV
    u235Table.addRecordR(NEUTRON, NEUTRON, 3, 5.0, 0.1);       // 100 keV
    u235Table.addRecordR(NEUTRON, NEUTRON, 3, 2.0, 0.5);       // 500 keV
    u235Table.addRecordR(NEUTRON, NEUTRON, 3, 1.2, 1.0);       // 1 MeV
    u235Table.addRecordR(NEUTRON, NEUTRON, 3, 1.0, 2.0);       // 2 MeV

    // Alpha particles - very high stopping power (high Z)
    u235Table.addRecordS(ALPHA, 8000.0, 0.01);
    u235Table.addRecordA(ALPHA, 800.0, 0.01);
    u235Table.addRecordS(ALPHA, 1600.0, 0.05);
    u235Table.addRecordA(ALPHA, 250.0, 0.05);
    u235Table.addRecordS(ALPHA, 650.0, 0.1);
    u235Table.addRecordA(ALPHA, 100.0, 0.1);
    u235Table.addRecordS(ALPHA, 250.0, 0.2);
    u235Table.addRecordA(ALPHA, 40.0, 0.2);
    u235Table.addRecordS(ALPHA, 80.0, 0.5);
    u235Table.addRecordA(ALPHA, 15.0, 0.5);
    u235Table.addRecordS(ALPHA, 40.0, 1.0);
    u235Table.addRecordA(ALPHA, 8.0, 1.0);
    u235Table.addRecordS(ALPHA, 15.0, 2.0);
    u235Table.addRecordA(ALPHA, 3.0, 2.0);

    // Beta particles - high stopping power
    u235Table.addRecordS(BETA, 800.0, 0.01);
    u235Table.addRecordA(BETA, 150.0, 0.01);
    u235Table.addRecordS(BETA, 250.0, 0.05);
    u235Table.addRecordA(BETA, 50.0, 0.05);
    u235Table.addRecordS(BETA, 80.0, 0.1);
    u235Table.addRecordA(BETA, 15.0, 0.1);
    u235Table.addRecordS(BETA, 30.0, 0.2);
    u235Table.addRecordA(BETA, 6.0, 0.2);
    u235Table.addRecordS(BETA, 12.0, 0.5);
    u235Table.addRecordA(BETA, 2.5, 0.5);
    u235Table.addRecordS(BETA, 6.0, 1.0);
    u235Table.addRecordA(BETA, 1.2, 1.0);

    // Gamma - high absorption (high Z, similar to Pb)
    u235Table.addRecordS(GAMMA, 6.0, 0.01);
    u235Table.addRecordA(GAMMA, 180.0, 0.01);
    u235Table.addRecordS(GAMMA, 9.0, 0.05);
    u235Table.addRecordA(GAMMA, 25.0, 0.05);
    u235Table.addRecordS(GAMMA, 12.0, 0.1);
    u235Table.addRecordA(GAMMA, 6.0, 0.1);
    u235Table.addRecordS(GAMMA, 10.0, 0.2);
    u235Table.addRecordA(GAMMA, 2.5, 0.2);
    u235Table.addRecordS(GAMMA, 6.0, 0.5);
    u235Table.addRecordA(GAMMA, 1.0, 0.5);
    u235Table.addRecordS(GAMMA, 4.0, 1.0);
    u235Table.addRecordA(GAMMA, 0.6, 1.0);
    u235Table.addRecordS(GAMMA, 2.5, 2.0);
    u235Table.addRecordA(GAMMA, 0.4, 2.0);

    // Neutrons - scattering plus high capture/fission
    // Total XS high at thermal energies due to fission
    // Capture (non-fission) XS: ~98 barns thermal
    u235Table.addRecordS(NEUTRON, 15.0, 0.01);
    u235Table.addRecordA(NEUTRON, 50.0, 0.01);    // High capture
    u235Table.addRecordS(NEUTRON, 12.0, 0.05);
    u235Table.addRecordA(NEUTRON, 20.0, 0.05);
    u235Table.addRecordS(NEUTRON, 10.0, 0.1);
    u235Table.addRecordA(NEUTRON, 5.0, 0.1);
    u235Table.addRecordS(NEUTRON, 8.0, 0.5);
    u235Table.addRecordA(NEUTRON, 0.5, 0.5);
    u235Table.addRecordS(NEUTRON, 7.0, 1.0);
    u235Table.addRecordA(NEUTRON, 0.2, 1.0);
    u235Table.addRecordS(NEUTRON, 6.0, 2.0);
    u235Table.addRecordA(NEUTRON, 0.1, 2.0);

    return Material{"U235", 235, 19.0, u235Table};
}

inline Material getM_Pu239() {
    // Plutonium-239 - fissile material
    // Higher fission cross-section than U235 at thermal energies
    // Density ~19.9 g/cm³
    XSTable pu239Table;

    // Reaction records
    pu239Table.addRecordR(ALPHA, ALPHA, 1, 0.0, 0.0);
    pu239Table.addRecordR(BETA, BETA, 1, 0.0, 0.0);
    pu239Table.addRecordR(GAMMA, GAMMA, 1, 0.0, 0.0);
    // Fission: n + Pu239 → fission products + ~2.9 neutrons (avg)
    // Thermal fission XS: ~748 barns, higher than U235
    pu239Table.addRecordR(NEUTRON, NEUTRON, 3, 748.0, 0.025);  // Thermal
    pu239Table.addRecordR(NEUTRON, NEUTRON, 3, 60.0, 0.01);    // 10 keV
    pu239Table.addRecordR(NEUTRON, NEUTRON, 3, 6.0, 0.1);      // 100 keV
    pu239Table.addRecordR(NEUTRON, NEUTRON, 3, 2.5, 0.5);      // 500 keV
    pu239Table.addRecordR(NEUTRON, NEUTRON, 3, 1.8, 1.0);      // 1 MeV
    pu239Table.addRecordR(NEUTRON, NEUTRON, 3, 1.5, 2.0);      // 2 MeV

    // Alpha particles - very high stopping power (high Z)
    pu239Table.addRecordS(ALPHA, 8500.0, 0.01);
    pu239Table.addRecordA(ALPHA, 850.0, 0.01);
    pu239Table.addRecordS(ALPHA, 1700.0, 0.05);
    pu239Table.addRecordA(ALPHA, 280.0, 0.05);
    pu239Table.addRecordS(ALPHA, 700.0, 0.1);
    pu239Table.addRecordA(ALPHA, 110.0, 0.1);
    pu239Table.addRecordS(ALPHA, 280.0, 0.2);
    pu239Table.addRecordA(ALPHA, 45.0, 0.2);
    pu239Table.addRecordS(ALPHA, 90.0, 0.5);
    pu239Table.addRecordA(ALPHA, 18.0, 0.5);
    pu239Table.addRecordS(ALPHA, 45.0, 1.0);
    pu239Table.addRecordA(ALPHA, 9.0, 1.0);
    pu239Table.addRecordS(ALPHA, 18.0, 2.0);
    pu239Table.addRecordA(ALPHA, 3.5, 2.0);

    // Beta particles - high stopping power
    pu239Table.addRecordS(BETA, 850.0, 0.01);
    pu239Table.addRecordA(BETA, 160.0, 0.01);
    pu239Table.addRecordS(BETA, 280.0, 0.05);
    pu239Table.addRecordA(BETA, 55.0, 0.05);
    pu239Table.addRecordS(BETA, 90.0, 0.1);
    pu239Table.addRecordA(BETA, 18.0, 0.1);
    pu239Table.addRecordS(BETA, 35.0, 0.2);
    pu239Table.addRecordA(BETA, 7.0, 0.2);
    pu239Table.addRecordS(BETA, 14.0, 0.5);
    pu239Table.addRecordA(BETA, 3.0, 0.5);
    pu239Table.addRecordS(BETA, 7.0, 1.0);
    pu239Table.addRecordA(BETA, 1.4, 1.0);

    // Gamma - high absorption (high Z)
    pu239Table.addRecordS(GAMMA, 6.5, 0.01);
    pu239Table.addRecordA(GAMMA, 190.0, 0.01);
    pu239Table.addRecordS(GAMMA, 10.0, 0.05);
    pu239Table.addRecordA(GAMMA, 28.0, 0.05);
    pu239Table.addRecordS(GAMMA, 13.0, 0.1);
    pu239Table.addRecordA(GAMMA, 7.0, 0.1);
    pu239Table.addRecordS(GAMMA, 11.0, 0.2);
    pu239Table.addRecordA(GAMMA, 3.0, 0.2);
    pu239Table.addRecordS(GAMMA, 7.0, 0.5);
    pu239Table.addRecordA(GAMMA, 1.2, 0.5);
    pu239Table.addRecordS(GAMMA, 4.5, 1.0);
    pu239Table.addRecordA(GAMMA, 0.7, 1.0);
    pu239Table.addRecordS(GAMMA, 3.0, 2.0);
    pu239Table.addRecordA(GAMMA, 0.45, 2.0);

    // Neutrons - scattering plus high capture/fission
    // Capture XS: ~271 barns thermal (higher than U235)
    pu239Table.addRecordS(NEUTRON, 10.0, 0.01);
    pu239Table.addRecordA(NEUTRON, 80.0, 0.01);    // Very high capture
    pu239Table.addRecordS(NEUTRON, 9.0, 0.05);
    pu239Table.addRecordA(NEUTRON, 30.0, 0.05);
    pu239Table.addRecordS(NEUTRON, 8.0, 0.1);
    pu239Table.addRecordA(NEUTRON, 8.0, 0.1);
    pu239Table.addRecordS(NEUTRON, 7.0, 0.5);
    pu239Table.addRecordA(NEUTRON, 0.8, 0.5);
    pu239Table.addRecordS(NEUTRON, 6.0, 1.0);
    pu239Table.addRecordA(NEUTRON, 0.3, 1.0);
    pu239Table.addRecordS(NEUTRON, 5.5, 2.0);
    pu239Table.addRecordA(NEUTRON, 0.15, 2.0);

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
