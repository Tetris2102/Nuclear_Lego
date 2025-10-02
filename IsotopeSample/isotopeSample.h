// isotopeSample.h

#ifndef IsotopeSample_H
#define IsotopeSample_H

#include "../Material/material.h"
#include <string>
#include <vector>
#include <array>
#include <random>

// Mass of source is considered constant
class IsotopeSample {
    private:
        string name;
        double activity;
        ParticleType emissionParticle;
        double emissionEnergy;
        Material surroundMaterial;

        // RNG for generateParticles()
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        void normalizeV(std::array<double, 3> &dirXYZ);
    public:
        // Not radioactive by default (0 decays per second)
        IsotopeSample(string name, short int atomicMass, double activity = 0.0,
          ParticleType emissionParticle, double emissionEnergy,
          Material surroundMaterial) : surroundMaterial(surroundMaterial) {
            this->name = name;
            this->activity = activity;
            this->emissionParticle = emissionParticle;
            this->emissionEnergy = emissionEnergy;
        }

        // Create a particle list with uniform distribution in space
        std::vector<Particle> generateParticles(double timeElapsed,
          std::array<double, 3> originXYZ);
        std::string getName();
        void setActivity(double a);
        double getActivity();
        void setEmitParticleType(ParticleType pt);
        ParticleType getEmitParticleType();
        Material getSurroundMaterial();
};

#endif
