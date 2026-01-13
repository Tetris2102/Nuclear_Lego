// xstable.cpp

#include "xstable.h"
#include <cmath>
#include <stdexcept>

// EventType XSTable::getEventType() {
//     return eventType;
// }
//
// ParticleType XSTable::getIncParticleGroupType() {
//     return incParticleGroup;
// }
//
// ParticleType XSTable::getFinalParticleGroupType() {
//     return finalParticleGroup;
// }
//
// int XSTable::getFinalParticleGroups() {
//     return finalParticleGroupCount;
// }
//
// void XSTable::addXS(float energy, float xs) {
//     eToXS[energy] = xs;
// }
//
// float XSTable::getXS(float energy) {
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

void XSTable::addRecordA(ParticleType incPT, float xs, float energy) {
    XSRecord r = {ABSORB, incPT, NONE, 0, xs, energy};
    records.push_back(r);
}

void XSTable::addRecordS(ParticleType incPT, float xs, float energy) {
    XSRecord r = {SCATTER, incPT, incPT, 1, xs, energy};
    records.push_back(r);
}

void XSTable::addRecordR(ParticleType incPT, ParticleType finPT,
  short int finPCount, float xs, float energy) {
    XSRecord r = {REACTION, incPT, finPT, finPCount, xs, energy};
    records.push_back(r);
}

XSRecord XSTable::findRecord(EventType et, const ParticleGroup& incP) const {
    std::vector<XSRecord> particleMatch;
    ParticleType type = incP.getType();
    float energy = incP.getEnergy();

    for(const XSRecord &r : records) {
        if(r.incParticleGroup == type) particleMatch.push_back(r);
    }

    // If no records found, return 0.0 cross-section if none found
    // ParticleGroup remains the same
    if(particleMatch.size() == 0) {
        return XSRecord{et, type, type, 1, 0.0, energy};
    }

    float diff;
    XSRecord match;
    for(XSRecord r : particleMatch) {
        if(std::abs(energy - r.energy) < diff || diff == 0) {
            match = r;
            diff = std::abs(energy - r.energy);
        }
    }
    return match;
}

// Find the best fitting record for each EventType of a given particle
std::array<XSRecord, 3> XSTable::findEventRecords(const ParticleGroup& incP) {
    std::array<XSRecord, 3> eventRecords;
    // Used to loop through EventType
    // Same order as in EventType definition
    std::array<EventType, 3> events = {ABSORB, SCATTER, REACTION};

    for(int i = 0; i<3; i++) {
        std::vector<XSRecord> firstMatches;
        ParticleType type = incP.getType();
        float energy = incP.getEnergy();

        // Match by incident particle and event type
        for(const XSRecord &r : records) {
            if(r.incParticleGroup == type && r.event == events[i]) {
                firstMatches.push_back(r);
            }
        }

        // If no records found, return 0.0 cross-section if none found
        // ParticleGroup remains the same
        if(firstMatches.size() == 0) {
            eventRecords[i] = XSRecord{events[i], type, type, 0, 0.0, energy};
            continue;
        }

        XSRecord match;
        float diff;
        for(XSRecord r : firstMatches) {
            if(std::abs(energy - r.energy) < diff || diff == 0) {
                match = r;
                diff = std::abs(energy - r.energy);
            }
        }
        eventRecords[i] = match;
    }
    return eventRecords;
}
