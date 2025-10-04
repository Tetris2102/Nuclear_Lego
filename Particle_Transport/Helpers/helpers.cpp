#include "../Particle/particle.h"
#include <array>

void normalizeVecXYZ(double &x, double &y, double &z) {
    double magnitude = std::sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

void normalizeVec3Arr(std::array<double, 3> &dirXYZ) {
    double magnitude = std::sqrt(dirXYZ[0] * dirXYZ[0] + dirXYZ[1] * dirXYZ[1] +
      dirXYZ[2] * dirXYZ[2]);
    dirXYZ[0] /= magnitude;
    dirXYZ[1] /= magnitude;
    dirXYZ[2] /= magnitude;
}

std::array<double, 3> pointAlongVec(std::array<double> origin,
  std::array<double, 3> dirXYZ, double tmax) {
    std::array<double, 3> pointAlongVec;
    pointAlongVec[0] = origin[0] + tmax * dirXYZ[0];
    pointAlongVec[1] = origin[1] + tmax * dirXYZ[1];
    pointAlongVec[2] = origin[2] + tmax * dirXYZ[2];
    return pointAlongVec;
}
