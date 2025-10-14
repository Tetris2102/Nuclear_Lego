// isotopeSample.cpp

#include "isotopeSample.h"
#include "../Helpers/helpers.cpp"
#include <string>
#include <vector>
#include <array>
#include <cmath>

std::vector<Particle> IsotopeSample::generateParticles(float timeElapsed,
  Vector3 originXYZ, std::uniform_real_distribution<float>& dist,
  std::mt19937& gen) {
    std::vector<Particle> particles;

    int decays = activity * timeElapsed;
    for(int i = 0; i<decays; i++) {
        // Convert random numbers from [0; 1) to [-1; 1) range
        Vector3 mom(dist(gen) * 2 - 1, dist(gen) * 2 - 1, dist(gen) * 2 - 1);
        mom.normalize();
        Particle p(emissionParticle, emissionEnergy, originXYZ, mom);
        particles.push_back(p);
    }
    return particles;
}

std::string IsotopeSample::getName() {
    return name;
}

void IsotopeSample::setActivity(float a) {
    activity = a;
}

float IsotopeSample::getActivity() {
    return activity;
}

void IsotopeSample::setEmitParticleType(ParticleType pt) {
    emissionParticle = pt;
}

ParticleType IsotopeSample::getEmitParticleType() {
    return emissionParticle;
}

float IsotopeSample::getEmitEnergy() {
  return emissionEnergy;
}

bool IsotopeSample::isRadioactive() {
    return (activity > 0.0);
}
