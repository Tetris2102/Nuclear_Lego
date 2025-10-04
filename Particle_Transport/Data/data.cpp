// data.cpp

#include "../Material/material.h"
#include "../XSTable/xstable.h"
#include "../Particle/particle.h"
#include "../IsotopeSample/isotopeSample.h"

// Energy values used: 10 keV, 50 keV, 100 keV, 200 keV, 500 keV, 1 MeV, 2 MeV

// Air (atmospheric pressure, approx. 78% N2 + 21% O2)

XSTable airTable();

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

Material air("air", 29.0, 0.0012, airTable);
