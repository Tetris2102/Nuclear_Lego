// isotopeSample.cpp

#include "isotopeSample.h"
#include "../Helpers/helpers.cpp"
#include <string>
#include <vector>
#include <array>
#include <cmath>

std::vector<Particle> IsotopeSample::generateParticles(double timeElapsed,
  Vector3 originXYZ, std::uniform_real_distribution<double>& dist,
  std::mt19937& gen) {
    std::vector<Particle> particles;

    int decays = activity * timeElapsed;
    for(int i = 0; i<decays; i++) {
        Vector3 mom(dist(gen), dist(gen), dist(gen));
        mom.normalize();
        Particle p(emissionParticle, emissionEnergy, originXYZ, mom);
        particles.push_back(p);
    }
    return particles;
  }

std::string IsotopeSample::getName() {
    return name;
}

void IsotopeSample::setActivity(double a) {
    activity = a;
}

double IsotopeSample::getActivity() {
    return activity;
}

void IsotopeSample::setEmitParticleType(ParticleType pt) {
    emissionParticle = pt;
}

ParticleType IsotopeSample::getEmitParticleType() {
    return emissionParticle;
}

double IsotopeSample::getEmitEnergy() {
  return emissionEnergy;
}

bool IsotopeSample::isRadioactive() {
    return (activity > 0.0);
}
