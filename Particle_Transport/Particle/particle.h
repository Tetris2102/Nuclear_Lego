// particle.h

#ifndef PARTICLE_H
#define PARTICLE_H

#include <array>

enum ParticleType {
    ALPHA,
    BETA,
    GAMMA,
    NEUTRON,
    PROTON
};

class Particle {
    private:
        ParticleType type;
        bool exists = true;
        double E = 0.0f;  // energy

        // Position
        std::array<double, 3> position = {0.0, 0.0, 0.0};
        // Momentum
        std::array<double, 3> mom = {0.0, 0.0, 0.0};
    public:
        Particle(ParticleType type, double energy, std::array<double, 3> position,
            std::array<double, 3> momentumXYZ) {
            this->type = type;
            this->E = energy;
            this->position = position;
            this->mom = momentumXYZ;
        }

        void setEnergy(double newE);
        double getEnergy();
        void setType(ParticleType newType);
        ParticleType getType();
        void deactivate();
        bool isActive();

        void setMomentum(std::array<double, 3> newMom);
        std::array<double, 3> getMomentum();
        double getMomX();
        double getMomY();
        double getMomZ();

        void setPosition(std::array<double, 3> newPos);
        std::array<double, 3> getPosition();
        double getX();
        double getY();
        double getZ();
};

#endif
