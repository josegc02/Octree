#include "OctreeNode.h"
#include <iostream>
#include <cmath>

OctreeNode::OctreeNode(const Point3D& c, double side, double minSide)
    : center(c), sideLength(side), minSideLength(minSide) {}

OctreeNode::~OctreeNode() {
    for (auto& child : children) {
        if (child) {
            delete child;
            child = nullptr;
        }
    }
}

std::vector<Point3D> search(OctreeNode* node){

}

bool OctreeNode::contains(const Point3D& p) const {
    double half = sideLength * 0.5;
    return (p.x >= center.x - half && p.x <= center.x + half) &&
           (p.y >= center.y - half && p.y <= center.y + half) &&
           (p.z >= center.z - half && p.z <= center.z + half);
}

bool OctreeNode::insert(const Point3D& p) {
    if (!contains(p)) return false;

    if (points.size() < MAX_POINTS or sideLength * 0.5 < minSideLength) {
        points.push_back(p);
        return true;
    }
    if (!children[0]) subdivide();

    for (auto* child : children) {
        if (child->insert(p)) return true;
    }

    // Si no se pudo insertar (raro), se queda aquí
    points.push_back(p);
    return true;
}

bool OctreeNode::remove(const Point3D& p) {
    if (!contains(p)) return false;

    for (auto it = points.begin(); it != points.end(); ++it) {
        if (*it == p) {
            points.erase(it);
            return true;
        }
    }

    if (children[0]) {
        for (auto* child : children) {
            if (child->remove(p)) {
                int totalPointsInChildren = 0;
                for (auto* c : children) {
                    if (c) totalPointsInChildren += static_cast<int>(c->points.size());
                }
                if (totalPointsInChildren < MAX_POINTS) {
                    merge();
                }
                return true;
            }
        }
    }
    return false;
}




void OctreeNode::getNearby(const Point3D& position, double maxDistance, std::vector<Point3D>& results) const {
    double half = sideLength * 0.5;
    double dist = 0.0;

    for (int i = 0; i < 3; i++) {
        double coord = (i == 0 ? position.x : (i == 1 ? position.y : position.z));
        double minB = (i == 0 ? center.x - half : (i == 1 ? center.y - half : center.z - half));
        double maxB = (i == 0 ? center.x + half : (i == 1 ? center.y + half : center.z + half));
        if (coord < minB) dist += (minB - coord) * (minB - coord);
        else if (coord > maxB) dist += (coord - maxB) * (coord - maxB);
    }
    if (dist > maxDistance * maxDistance) return;

    double maxDistSqr = maxDistance * maxDistance;
    for (const auto& p : points) {
        double dx = p.x - position.x;
        double dy = p.y - position.y;
        double dz = p.z - position.z;
        double d2 = dx * dx + dy * dy + dz * dz;
        if (d2 <= maxDistSqr) {
            results.push_back(p);
        }
    }

    if (children[0]) {
        for (auto* child : children) {
            child->getNearby(position, maxDistance, results);
        }
    }
}

void OctreeNode::print(int depth) const {
    std::string indent(depth * 2, ' ');
    std::cout << indent << "Node center: (" << center.x << ", " << center.y << ", " << center.z
              << "), sideLength: " << sideLength << "\n";
    std::cout << indent << "Points: " << points.size() << "\n";
    for (const auto& p : points) {
        std::cout << indent << "  (" << p.x << ", " << p.y << ", " << p.z << ")\n";
    }
    if (children[0]) {
        std::cout << indent << "Children:\n";
        for (int i = 0; i < 8; i++) {
            if (children[i]) {
                children[i]->print(depth + 1);
            }
        }
    }
}

void OctreeNode::subdivide() {
    double quarter = sideLength * 0.25;
    double childSide = sideLength * 0.5;
    for (int dx = -1; dx <= 1; dx += 2)
        for (int dy = -1; dy <= 1; dy += 2)
            for (int dz = -1; dz <= 1; dz += 2) {
                Point3D newCenter{
                    center.x + dx * quarter,
                    center.y + dy * quarter,
                    center.z + dz * quarter};
                int idx = ((dx > 0) << 2) | ((dy > 0) << 1) | (dz > 0);
//                int x; int y; int z;
//                if (dx < 0)  x = 0;
//                else x = 1;
//                if (dy < 0) y = 0;
//                else y = 1;
//                if (dz < 0) z = 0;
//                else z = 1;
//                int idx = x*4 + y*2 + z;
                children[idx] = new OctreeNode(newCenter, childSide, minSideLength);
            }

    std::vector<Point3D> oldPoints = points;
    points.clear();
    for (const auto& p : oldPoints) {
        bool inserted = false;
        for (auto* child : children) {
            if (child->insert(p)) {
                inserted = true;
                break;
            }
        }
        if (!inserted) points.push_back(p); //salvaguardar
    }
}

void OctreeNode::collectAllPoints(std::vector<Point3D>& collector) const {
    collector.insert(collector.end(), points.begin(), points.end());

    if (children[0]) {
        for (int i = 0; i < 8; ++i) {
            if (children[i]) {
                children[i]->collectAllPoints(collector);
            }
        }
    }
}

void OctreeNode::merge() {
    if (!children[0]) return;

    std::vector<Point3D> allPoints;
    for (int i = 0; i < 8; ++i) {
        if (children[i]) {
            children[i]->collectAllPoints(allPoints);
            delete children[i];
            children[i] = nullptr;
        }
    }

    points.insert(points.end(), allPoints.begin(), allPoints.end());
}
