//
// Created by esteb on 17/05/2025.
//

#ifndef POINT3D_H
#define POINT3D_H

struct Point3D {
    double x;
    double y;
    double z;
    Point3D() : x(0), y(0), z(0) {}
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
    bool operator==(const Point3D& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }
};

#endif //POINT3D_H
