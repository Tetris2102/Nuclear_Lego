// isotopeSample.cpp

#include "isotopeSample.h"
#include "../Helpers/helpers.cpp"
#include <string>
#include <vector>
#include <array>
#include <cmath>

std::vector<ParticleGroup> IsotopeSample::generateParticleGroups(
  float timeElapsed, Vector3 originXYZ, uint16_t partGroupSize,
  std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
    std::vector<ParticleGroup> particles;

    int decays = activity * timeElapsed;
    for(int i = 0; i<decays; i += partGroupSize) {
        // Convert random numbers from [0; 1) to [-1; 1) range
        Vector3 mom(dist(gen) * 2 - 1, dist(gen) * 2 - 1, dist(gen) * 2 - 1);
        mom.normalize();
        ParticleGroup p(emissionParticleGroup, emissionEnergy, originXYZ, mom);
        p.setGroupSize(partGroupSize);
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

void IsotopeSample::setEmitParticleGroupType(ParticleType pt) {
    emissionParticleGroup = pt;
}

ParticleType IsotopeSample::getEmitParticleGroupType() {
    return emissionParticleGroup;
}

float IsotopeSample::getEmitEnergy() {
  return emissionEnergy;
}

bool IsotopeSample::isRadioactive() {
    return (activity > 0.0);
}
