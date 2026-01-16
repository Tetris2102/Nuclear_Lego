// vector3.h

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

struct Vector3 {
    float x, y, z;

    Vector3(float _x=0.0, float _y=0.0, float _z=0.0) : x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float a) const;  // Multiply every component by a
    float dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    Vector3 operator/(const Vector3& v) const;
    void normalize();
    // Creates randomly chosen orthogonal vector (orthogonal to this Vector3)
    // Uses random u in range [0; 1) for x, y and z
    Vector3 randomOrthogonalV3(float u_x, float u_y, float u_z) const;
};

// Is it needed?
// struct Point3 {
//     float x, y, z;
//     Point3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
// }

#endif
