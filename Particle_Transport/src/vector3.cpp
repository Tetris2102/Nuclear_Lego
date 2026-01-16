// vector3.cpp

#include "vector3.h"

Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(float a) const {
    return Vector3(x * a, y * a, z * a);
}

float Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const {
    // Correct cross product: (y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x)
    return Vector3(y * v.z - z * v.y,
                   z * v.x - x * v.z,
                   x * v.y - y * v.x);
}

Vector3 Vector3::operator/(const Vector3& v) const {
    return Vector3(x / v.x, y / v.y, z / v.z);
}

void Vector3::normalize() {
    float magnitude = std::sqrt(x*x + y*y + z*z);
    if(magnitude == 0.0f) {
        // Avoid division by zero. Leave vector unchanged or set to a default unit
        // vector. We choose to set to (1,0,0) to ensure downstream code sees
        // a valid normalized vector instead of NaNs.
        x = 1.0f; y = 0.0f; z = 0.0f;
        return;
    }
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

Vector3 Vector3::randomOrthogonalV3(float u_x, float u_y, float u_z) const {
    // Create random vector with each component in [-1; 1] range (initial range [0; 1))
    Vector3 vec3(u_x * 2 - 1, u_y * 2 - 1, u_z * 2 - 1);
    // Make it orthogonal to current object
    // *this is the current object
    return vec3 - *this * dot(vec3);
}
