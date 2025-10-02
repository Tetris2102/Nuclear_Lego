// isotopeSample.cpp

#include "isotopeSample.h"
#include <string>
#include <vector>
#include <array>
#include <cmath>

void normalizeV(std::array<double, 3> &dirXYZ) {
    double magnitude = std::sqrt(dirXYZ[0] * dirXYZ[0] + dirXYZ[1] * dirXYZ[1] +
      dirXYZ[2] * dirXYZ[2]);
    dirXYZ[0] /= magnitude;
    dirXYZ[1] /= magnitude;
    dirXYZ[2] /= magnitude;
}

std::vector<Particle> IsotopeSample::generateParticles(double timeElapsed,
  std::array<double, 3> originXYZ) {
    std::vector<Particle> particles;

    int decays = activity * timeElapsed;
    for(int i = 0; i<decays; i++) {
        std::array<double, 3> mom = {distribution(generator),
          distribution(generator), distribution(generator)};
        normalizeV(mom);
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

Material IsotopeSample::getSurroundMaterial() {
    return surroundMaterial;
}
