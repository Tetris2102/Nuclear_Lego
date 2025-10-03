#include "../Particle/particle.h"

void normalizeVecXYZ(double &x, double &y, double &z) {
    double magnitude = std::sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}
