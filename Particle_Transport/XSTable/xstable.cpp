// xstable.cpp

#include "xstable.h"
#include <cmath>

// EventType XSTable::getEventType() {
//     return eventType;
// }
//
// ParticleType XSTable::getIncParticleType() {
//     return incParticle;
// }
//
// ParticleType XSTable::getFinalParticleType() {
//     return finalParticle;
// }
//
// int XSTable::getFinalParticles() {
//     return finalParticleCount;
// }
//
// void XSTable::addXS(double energy, double xs) {
//     eToXS[energy] = xs;
// }
//
// double XSTable::getXS(double energy) {
//     auto it = eToXS.lower_bound(energy);
//
//     if (it == eToXS.begin()) {
//       return it->second;
//     } else if (it == eToXS.end()) {
//       return (it--)->second;
//     } else {
//       auto prevIt = std::prev(it);
//       if (std::abs(energy - it->first) < std::abs(energy - prevIt->first)) {
//         return it->second;
//       } else {
//         return prevIt->second;
//       }
//     }
// }

void XSTable::addRecord(EventType evType, ParticleType incPT, ParticleType finPT,
  short int finPCount, double xs, double energy) {
    XSRecord r = {evType, incPT, finPT, finPCount, xs, energy};
    records.push_back(r);
  }

XSRecord XSTable::findRecord(EventType et, Particle incP) {
    std::vector<XSRecord> particleMatch;
    ParticleType type = incP.getType();
    double energy = incP.getEnergy();

    for(const XSRecord &r : records) {
        if(r.incParticle == type) particleMatch.push_back(r);
    }

    // If no records found, return 0.0 cross-section if none found
    // Particle remains the same
    if(particleMatch.size() == 0) return XSRecord{et, type, type, 0, 0.0, energy};

    double diff = std::numeric_limits<double>::infinity();
    XSRecord match;
    for(XSRecord r : particleMatch) {
        if(std::abs(energy - r.energy) < diff) {
            match = r;
            diff = std::abs(energy - r.energy);
        }
    }
    return match;
}

// Find the best fitting record for each EventType of a given particle
std::array<XSRecord, 3> XSTable::findEventRecords(Particle incP) {
    std::array<XSRecord, 3> eventRecords;
    // Used to loop through EventType
    // Same order as in EventType definition
    std::array<EventType, 3> events = {ABSORB, SCATTER, REACTION};

    for(int i = 0; i<3; i++) {
        std::vector<XSRecord> firstMatches;
        ParticleType type = incP.getType();
        double energy = incP.getEnergy();

        // Match by incident particle and event type
        for(const XSRecord &r : records) {
            if(r.incParticle == type && r.event == events[i]) {
                firstMatches.push_back(r);
            }
        }

        // If no records found, return 0.0 cross-section if none found
        // Particle remains the same
        if(firstMatches.size() == 0) {
            eventRecords[i] = XSRecord{events[i], type,
              type, 0, 0.0, energy};
            continue;
        }

        XSRecord match;
        double diff = std::numeric_limits<double>::infinity();
        for(XSRecord r : firstMatches) {
            if(std::abs(energy - r.energy) < diff) {
                match = r;
                diff = std::abs(energy - r.energy);
            }
        }
        eventRecords[i] = match;
    }
    return eventRecords;
}
