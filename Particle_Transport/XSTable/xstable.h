// xstable.h

#ifndef EVENT_H
#define EVENT_H

#include "../ParticleGroup/particleGroup.h"
#include <vector>
#include <array>
#include <utility>
// #include <map>

enum EventType {
    ABSORB,
    SCATTER,
    REACTION
};

// class XSTable {
//     private:
//         EventType eventType;
//         ParticleType incParticleGroup;  // Incident particle type
//         ParticleType finalParticleGroup;  // Type of particle produced
//         int finalParticleGroupCount;  // How many final particles are emitted
//
//         // Energy to cross-section map
//         // All cross-sections are macroscopic
//         std::map<float, float> eToXS;
//     public:
//         XSTable(EventType eventType, ParticleType incParticleGroup,
//           ParticleType finalParticleGroup=NONE, int finalParticleGroupCount) {
//             this->eventType = eventType;
//             this->incParticleGroup = incParticleGroup;
//             this->finalParticleGroup = finalParticleGroup;
//             this->finalParticleGroupCount = finalParticleGroupCount;
//         }
//
//         EventType getEventType();
//         ParticleType getIncParticleGroupType();
//         ParticleType getFinalParticleGroupType();
//         int getFinalParticleGroups();
//         void addXS(float energy, float xs);
//         float getXS(float energy);
// };

struct XSRecord {
    EventType event;
    ParticleType incParticleGroup;
    ParticleType finalParticleGroup;
    short int finalParticleGroupCount;  // How many particles in total after event
    float xs;
    float energy;
};

class XSTable {
    private:
        std::vector<XSRecord> records;
    public:
        XSTable() {}

        // ABSORB
        void addRecordA(ParticleType incPT, float xs, float energy);
        // SCATTER
        void addRecordS(ParticleType incPT, float xs, float energy);
        // REACTION
        void addRecordR(ParticleType incPT, ParticleType finPT,
          short int finPCount, float xs, float energy);
        XSRecord findRecord(EventType et, const ParticleGroup& incP) const;
        // Finds the best record for every EventType
        std::array<XSRecord, 3> findEventRecords(const ParticleGroup& incP);
};

#endif
