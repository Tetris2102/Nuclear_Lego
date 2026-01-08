// #include "../Particle/particle.h"
#include "../Vector3/vector3.h"
#include <cmath>
#include <array>

inline void normalizeVecXYZ(float &x, float &y, float &z) {
    float magnitude = std::sqrt(x*x + y*y + z*z);
    if(magnitude == 0.0f) return; // leave as-is to avoid NaN
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

inline void normalizeVec3(Vector3& vec3) {
    float x, y, z;
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;

    float magnitude = std::sqrt(x*x + y*y + z*z);
    if(magnitude == 0.0f) return;
    vec3.x /= magnitude;
    vec3.y /= magnitude;
    vec3.z /= magnitude;
}
