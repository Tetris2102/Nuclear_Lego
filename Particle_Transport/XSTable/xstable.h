// xstable.h

#ifndef EVENT_H
#define EVENT_H

#include "../Particle/particle.h"
#include <map>

enum EventType {
    SCATTER,
    ABSORB,
    FISSION
};

class XSTable {
    private:
        EventType eventType;
        ParticleType incParticle;  // Incident particle type
        ParticleType finalParticle;  // Type of particle produced
        int finalParticleCount;  // How many final particles are emitted

        // Energy to cross-section map
        // All cross-sections are macroscopic
        std::map<double, double> eToXS;
    public:
        XSTable(EventType eventType, ParticleType incParticle,
          ParticleType finalParticle, int finalParticleCount) {
            this->eventType = eventType;
            this->incParticle = incParticle;
            this->finalParticle = finalParticle;
            this->finalParticleCount = finalParticleCount;
        }

        EventType getEventType();
        ParticleType getIncParticleType();
        ParticleType getFinalParticleType();
        int getFinalParticles();
        void addXS(double energy, double xs);
        double getXS(double energy);
};

#endif
