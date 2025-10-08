// CURRENTLY NOT USED (Oct. 6, 2025)

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

struct Vector3 {
    double x, y, z;

    Vector3(double _x=0.0, double _y=0.0, double _z=0.0) : x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(double a) const;  // Multiply every component by a
    double dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    Vector3 operator/(const Vector3& v) const;
    void normalize();
    // Creates randomly chosen orthogonal vector (orthogonal to this Vector3)
    // Uses random u in range [0; 1) for x, y and z
    Vector3 randomOrthogonalV3(double u_x, double u_y, double u_z) const;
};

// Is it needed?
// struct Point3 {
//     double x, y, z;
//     Point3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
// }

#endif
