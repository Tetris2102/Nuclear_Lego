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
        double getEnergy();
        void setType(ParticleType newType);
        ParticleType getType();
        void deactivate();
        bool isActive();

        void setMomentum(Vector3 newMom);
        Vector3 getMomentum();
        double getMomX();
        double getMomY();
        double getMomZ();

        void setPosition(Vector3 newPos);
        Vector3 getPosition();
        double getX();
        double getY();
        double getZ();

        Vector3 pointAlongVec(Vector3, Vector3 dirXYZ, double tmax);
        void moveToPointAlong(Particle& p, double t);
};

#endif
