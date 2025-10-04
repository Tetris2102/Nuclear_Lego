// isotopeSample.h

#ifndef IsotopeSample_H
#define IsotopeSample_H

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

        // RNG for generateParticles()
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        void normalizeV(std::array<double, 3> &dirXYZ);
    public:
        // Not radioactive by default (0 decays per second)
        IsotopeSample(string name, ParticleType emissionParticle,
          double emissionEnergy, double activity = 0.0) {
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
        double getEmitEnergy();
};

#endif
