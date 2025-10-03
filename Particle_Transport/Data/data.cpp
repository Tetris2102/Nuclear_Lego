#include "../Material/material.h"
#include "../XSTable/xstable.h"
#include "../Particle/particle.h"

// Cross-section tables
// Pb-207
XSTable pb207table();
pb207table.addRecord(ABSORB, ALPHA, NONE, 0, 1000.0, 2.0);
pb207table.addRecord(ABSORB, BETA, NONE, 0, 200.0, 0.3);
pb207table.addRecord(ABSORB, GAMMA, )
XSTable pb207Abeta(ABSORB, BETA, NONE, 0);
pb207Abeta.addXS(1.0, )
