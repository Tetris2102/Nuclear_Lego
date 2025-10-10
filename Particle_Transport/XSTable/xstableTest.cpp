#include "xstable.h"
#include "../Material/material.h"
#include "../Particle/particle.h"
#include <iostream>
#include <cmath>
#include <array>

using namespace std;

int main() {
    XSTable myTable{};
    myTable.addRecordA(BETA, 2.1, 1.4);
    myTable.addRecordS(BETA, 20.0, 1.3);
    myTable.addRecordR(BETA, GAMMA, 0, 1.7, 1.7);
    Material Pb207("Pb207", 207, 13.0, myTable);
    Particle beta(BETA, 1.4, {0.5, 0.9, 0.05}, {0.0, 0.2, 0.9});
    cout << "Absorption cross-section: " << Pb207.getXS(ABSORB, beta) <<
      " barns." << endl;
    array<XSRecord, 3> records = myTable.findEventRecords(beta);
    array<float, 3> xss = {records[0].xs, records[1].xs, records[2].xs};
    cout << "Cross-sections for absorb, scatter and react (barns): " <<
      xss[0] << ", " << xss[1] << " and " << xss[2] << endl;
    return 0;
}
