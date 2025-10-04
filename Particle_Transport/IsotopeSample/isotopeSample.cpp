// isotopeSample.cpp

#include "isotopeSample.h"
#include "../Helpers/helpers/h"
#include <string>
#include <vector>
#include <array>
#include <cmath>

std::vector<Particle> IsotopeSample::generateParticles(double timeElapsed,
  std::array<double, 3> originXYZ) {
    std::vector<Particle> particles;

    int decays = activity * timeElapsed;
    for(int i = 0; i<decays; i++) {
        std::array<double, 3> mom = {dist(gen),
          dist(gen), dist(gen)};
        normalizeVec3Arr(mom);
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
