// xstable.h

#ifndef EVENT_H
#define EVENT_H

#include "../Particle/particle.h"
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
//         ParticleType incParticle;  // Incident particle type
//         ParticleType finalParticle;  // Type of particle produced
//         int finalParticleCount;  // How many final particles are emitted
//
//         // Energy to cross-section map
//         // All cross-sections are macroscopic
//         std::map<float, float> eToXS;
//     public:
//         XSTable(EventType eventType, ParticleType incParticle,
//           ParticleType finalParticle=NONE, int finalParticleCount) {
//             this->eventType = eventType;
//             this->incParticle = incParticle;
//             this->finalParticle = finalParticle;
//             this->finalParticleCount = finalParticleCount;
//         }
//
//         EventType getEventType();
//         ParticleType getIncParticleType();
//         ParticleType getFinalParticleType();
//         int getFinalParticles();
//         void addXS(float energy, float xs);
//         float getXS(float energy);
// };

struct XSRecord {
    EventType event;
    ParticleType incParticle;
    ParticleType finalParticle;
    short int finalParticleCount;  // How many particles in total after event
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
        XSRecord findRecord(EventType et, const Particle& incP) const;
        // Finds the best record for every EventType
        std::array<XSRecord, 3> findEventRecords(const Particle& incP);
};

#endif
