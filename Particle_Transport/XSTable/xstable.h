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
//         std::map<double, double> eToXS;
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
//         void addXS(double energy, double xs);
//         double getXS(double energy);
// };

struct XSRecord {
    EventType event;
    ParticleType incParticle;
    ParticleType finalParticle;
    short int finalParticleCount;
    double xs;
    double energy;
};

class XSTable {
    private:
        std::vector<XSRecord> records;
    public:
        XSTable() {}

        void addRecord(EventType evType, ParticleType incPT, ParticleType finPT,
          short int finPCount, double xs, double energy);
        XSRecord findRecord(EventType et, Particle incP);
        // Finds the best record for every EventType
        std::array<XSRecord, 3> findEventRecords(Particle incP);
};

#endif
