// particleGroup.h

#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Vector3/vector3.h"
#include <array>
#include <cstdint>

enum ParticleType {
    NONE,
    ALPHA,
    BETA,
    GAMMA,
    NEUTRON
};

class ParticleGroup {
    private:
        ParticleType type;
        bool exists = true;
        float E = 0.0f;  // energy
        uint16_t numParticles = 1;

        // Position
        Vector3 position = {0.0, 0.0, 0.0};
        // Momentum
        Vector3 mom = {0.0, 0.0, 0.0};
    public:
        ParticleGroup(ParticleType _type, float _energy, Vector3 _position,
          Vector3 _momentumXYZ) {
            type = _type;
            E = _energy;
            position = _position;
            mom = _momentumXYZ;
        }

        // Empty constructor for initializing particle properties later
        ParticleGroup() {};

        void setEnergy(float newE);
        float getEnergy() const;
        void setType(ParticleType newType);
        ParticleType getType() const;
        void deactivate();
        bool isActive() const;

        void setMomentum(Vector3 newMom);
        Vector3 getMomentum() const;
        float getMomX() const;
        float getMomY() const;
        float getMomZ() const;

        void setPosition(Vector3 newPos);
        Vector3 getPosition() const;
        float getX() const;
        float getY() const;
        float getZ() const;

        void setGroupSize(uint16_t newNumParticles);
        uint16_t getGroupSize();

        Vector3 pointAlongVec(float t) const;
        void moveToPointAlong(float t);
};

#endif
