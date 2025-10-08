// particle.h

#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Vector3/vector3.h"
#include <array>

enum ParticleType {
    NONE,
    ALPHA,
    BETA,
    GAMMA,
    NEUTRON
};

class Particle {
    private:
        ParticleType type;
        bool exists = true;
        double E = 0.0f;  // energy

        // Position
        Vector3 position = {0.0, 0.0, 0.0};
        // Momentum
        Vector3 mom = {0.0, 0.0, 0.0};
    public:
        Particle(ParticleType _type, double _energy, Vector3 _position,
          Vector3 _momentumXYZ) {
            type = _type;
            E = _energy;
            position = _position;
            mom = _momentumXYZ;
        }

        void setEnergy(double newE);
        double getEnergy() const;
        void setType(ParticleType newType);
        ParticleType getType() const;
        void deactivate();
        bool isActive() const;

        void setMomentum(Vector3 newMom);
        Vector3 getMomentum() const;
        double getMomX() const;
        double getMomY() const;
        double getMomZ() const;

        void setPosition(Vector3 newPos);
        Vector3 getPosition() const;
        double getX() const;
        double getY() const;
        double getZ() const;

        Vector3 pointAlongVec(double t) const;
        void moveToPointAlong(double t);
};

#endif
