// isotopeSample.h

#ifndef IsotopeSample_H
#define IsotopeSample_H

#include "../Vector3/vector3.h"
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
        bool radioactive = true;

        // RNG for generateParticles()
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        void normalizeV(Vector3 &dirXYZ);
    public:
        // For radioactive samples
        IsotopeSample(string _name, ParticleType _emissionParticle,
          double _emissionEnergy, double _activity = 0.0) {
            name = _name;
            activity = _activity;
            emissionParticle = _emissionParticle;
            emissionEnergy = _emissionEnergy;
        }

        // For stable samples
        IsotopeSample() {
            isRadioactive = false;
        }

        // Create a particle list with uniform distribution in space
        std::vector<Particle> generateParticles(double timeElapsed,
          Vector3 originXYZ);
        std::string getName();
        void setActivity(double a);
        double getActivity();
        void setEmitParticleType(ParticleType pt);
        ParticleType getEmitParticleType();
        double getEmitEnergy();
};

#endif
