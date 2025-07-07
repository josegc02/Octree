#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <vector>
#include "Point3D.h"

class OctreeNode {
public:
    Point3D center;
    double sideLength;    // Longitud total de cada arista del cubo
    double minSideLength; // Límite mínimo para subdividir

    static constexpr int MAX_POINTS = 4;

    std::vector<Point3D> points;
    OctreeNode* children[8] = {nullptr};

    OctreeNode(const Point3D& c, double side, double minSide);
    ~OctreeNode();

    bool contains(const Point3D& p) const;
    bool insert(const Point3D& p);
    bool remove(const Point3D& p);
    void getNearby(const Point3D& position, double maxDistance, std::vector<Point3D>& results) const;
    void print(int depth = 0) const;
    void subdivide();
    void collectAllPoints(std::vector<Point3D>& collector) const;
    void merge();
    bool searchPoint(const Point3D& p) const;
    bool updatePoint(const Point3D& oldPos, const Point3D& newPos);

};

#endif // OCTREENODE_H
