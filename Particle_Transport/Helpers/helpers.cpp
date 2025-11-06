#include "../Particle/particle.h"
#include <array>

inline void normalizeVecXYZ(float &x, float &y, float &z) {
    float magnitude = std::sqrt(x*x + y*y + z*z);
    if(magnitude == 0.0f) return; // leave as-is to avoid NaN
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}
