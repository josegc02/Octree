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

bool OctreeNode::contains(const Point3D& p) const {
    double half = sideLength * 0.5;
    return (p.x >= center.x - half && p.x <= center.x + half) &&
           (p.y >= center.y - half && p.y <= center.y + half) &&
           (p.z >= center.z - half && p.z <= center.z + half);
}

bool OctreeNode::insert(const Point3D& p) {
    if (!contains(p)) return false;

    if (points.size() < MAX_POINTS || sideLength * 0.5 < minSideLength) {
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
    if (!contains(p)) return false; // caso base: si el nodo actual NO contiene el punto

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
                    totalPointsInChildren += static_cast<int>(c->points.size());
                }
                if (totalPointsInChildren + points.size() < MAX_POINTS) { // if can merge
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
        double coord = (i == 0 ? position.x : (i == 1 ? position.y : position.z)); // que coordenada estamos evalu&&o (x, y, z)
        double minB = (i == 0 ? center.x - half : (i == 1 ? center.y - half : center.z - half));
        double maxB = (i == 0 ? center.x + half : (i == 1 ? center.y + half : center.z + half));
        if (coord < minB) dist += (minB - coord) * (minB - coord);
        else if (coord > maxB) dist += (coord - maxB) * (coord - maxB);
    }
    if (dist > maxDistance * maxDistance) return; // golazo (area podada)

    double maxDistSqr = maxDistance * maxDistance;
    for (const auto& p : points) { // distancia entre SUS PUNTOS y el CENTRO (del radio de búsqueda)
        double dx = p.x - position.x;
        double dy = p.y - position.y;
        double dz = p.z - position.z;
        double d2 = dx * dx + dy * dy + dz * dz;
        if (d2 <= maxDistSqr) {
            results.push_back(p);
        }
    }

    if (children[0]) { // si tienes hijos, podas
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
    // CREAMOS LOS HIJOS
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
                children[idx] = new OctreeNode(newCenter, childSide, minSideLength);
            }

    // TRANSFERIR LOS "OBJETOS" DEL PADRE A LOS HIJOS
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
        if (!inserted) points.push_back(p);
    }
}

void OctreeNode::collectAllPoints(std::vector<Point3D>& collector) const {
    collector.insert(collector.end(), points.begin(), points.end());

    if (children[0]) {
        for (int i = 0; i < 8; ++i) {
            if (children[i]) { // --
                children[i]->collectAllPoints(collector);
            }
        }
    }
}

void OctreeNode::merge() {
    if (!children[0]) return; // ---

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

static bool searchRecursive(const OctreeNode* node, const Point3D& p) {
    if (!node || !node->contains(p)) return false;

    // 1) Esta en este nodo hoja?
    for (const auto& q : node->points)
        if (q == p) return true;

    // 2) Si hay hijos, probar en el hijo que lo contiene
    if (node->children[0]) {
        int idx = ((p.x > node->center.x) << 2) |
                  ((p.y > node->center.y) << 1) |
                  ( p.z > node->center.z);
        return searchRecursive(node->children[idx], p);
    }
    return false;
}

bool OctreeNode::searchPoint(const Point3D& p) const {
    if (!contains(p)) return false;               // fuera de este nodo

    // 1) ¿Está en este nodo?
    for (const auto& q : points)
        if (q == p) return true;

    // 2) Bajar al hijo adecuado (si existe subdivisión)
    if (children[0]) {
        int idx = ((p.x > center.x) << 2) |
                  ((p.y > center.y) << 1) |
                  ( p.z > center.z);
        return children[idx]->searchPoint(p);
    }
    return false;                                 // hoja y no lo encontramos
}


bool OctreeNode::updatePoint(const Point3D& oldPos,
                             const Point3D& newPos) {
    // Ambas posiciones deben caber en el cubo raíz (this):
    if (!contains(oldPos) || !contains(newPos)) return false;

    // 1) Elimina el punto antiguo
    if (!remove(oldPos)) return false;

    // 2) Inserta el nuevo
    if (!insert(newPos)) {        // ← solo fallaria si newPos trae duplicados
        insert(oldPos);           // rollback para no perder datos
        return false;
    }
    return true;
}

