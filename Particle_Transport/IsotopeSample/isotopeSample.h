// isotopeSample.h

#ifndef IsotopeSample_H
#define IsotopeSample_H

#include "../Vector3/vector3.h"
#include "../Particle/particle.h"
#include <string>
#include <vector>
#include <random>

// Mass of source is considered constant
class IsotopeSample {
    private:
        std::string name;
        double activity;
        ParticleType emissionParticle;
        double emissionEnergy;

        void normalizeV(Vector3 &dirXYZ);
    public:
        // For radioactive samples
        IsotopeSample(std::string _name, ParticleType _emissionParticle,
          double _emissionEnergy, double _activity = 0.0) {
            name = _name;
            activity = _activity;
            emissionParticle = _emissionParticle;
            emissionEnergy = _emissionEnergy;
        }

        // For stable samples
        IsotopeSample() {
            activity = 0.0;
        }

        // IsotopeSample(const IsotopeSample&) = delete;
        // IsotopeSample& operator=(const IsotopeSample&) = delete;
        //
        // IsotopeSample(IsotopeSample&&) noexcept = default;
        // IsotopeSample& operator=(IsotopeSample&&) noexcept = default;

        // Create a particle list with uniform distribution in space
        std::vector<Particle> generateParticles(double timeElapsed,
          Vector3 originXYZ, std::uniform_real_distribution<double>& dist,
          std::mt19937& gen);
        std::string getName();
        void setActivity(double a);
        double getActivity();
        void setEmitParticleType(ParticleType pt);
        ParticleType getEmitParticleType();
        double getEmitEnergy();
        bool isRadioactive();
};

#endif
