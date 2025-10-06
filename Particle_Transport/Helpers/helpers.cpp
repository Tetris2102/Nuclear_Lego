#include "../Particle/particle.h"
#include <array>

void normalizeVecXYZ(double &x, double &y, double &z) {
    double magnitude = std::sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

Vector3 pointAlongVec(Vector3,
  Vector3 dirXYZ, double tmax) {
    Vector3 pointAlongVec;
    pointAlongVec.x = origin.x + tmax * dirXYZ.x;
    pointAlongVec.y = origin.y + tmax * dirXYZ.y;
    pointAlongVec.z = origin.z + tmax * dirXYZ.z;
    return pointAlongVec;
}

void moveToPointAlong(Particle& p, double t) {
    p.setPosition(pointAlongVec(p.getPosition(), p.getMomentum(), t));
}
