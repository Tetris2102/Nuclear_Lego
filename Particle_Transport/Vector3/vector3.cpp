#include "vector3.h"

Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(double a) const {
    return Vector3(x * a, y * a, z * a);
}

double Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y, x * v.z - z * v.z, x * v.y - y * v.x);
}

Vector3 Vector3::operator/(const Vector3& v) const {
    return Vector3(x / v.x, y / v.y, z / v.z);
}

void Vector3::normalize() {
    double magnitude = std::sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

Vector3 Vector3::randomOrthogonalV3(double u_x, double u_y, double u_z) const {
    // Create random vector with each component in [-1; 1] range (initial range [0; 1))
    Vector3 vec3(u_x * 2 - 1, u_y * 2 - 1, u_z * 2 - 1);
    // Make it orthogonal to current object
    // *this is the current object
    return vec3 - *this * dot(vec3);
}
