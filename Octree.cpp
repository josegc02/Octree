//
// Created by esteb on 17/05/2025.
//

#include "Octree.h"

// Constructor
Octree::Octree(const Point3D& center, double sideLength, double minSide)
    : minSideLength(minSide)
{
    root = new OctreeNode(center, sideLength, minSideLength);
}

// Destructor
Octree::~Octree() {
    delete root;
}

// Insertar punto
bool Octree::insert(const Point3D& p) {
    return root->insert(p);
}

// Eliminar punto
bool Octree::remove(const Point3D& p) {
    return root->remove(p);
}

// Consultar puntos cercanos
std::vector<Point3D> Octree::getNearby(const Point3D& position, double maxDistance) const {
    std::vector<Point3D> results;
    root->getNearby(position, maxDistance, results);
    return results;
}

// Imprimir estructura
void Octree::print() const {
    root->print();
}

const OctreeNode * Octree::getRoot() const {
    return root;

}





bool Octree::searchPoint(const Point3D& p) const {
    return root ? root->searchPoint(p) : false;
}

bool Octree::updatePoint(const Point3D& oldPos, const Point3D& newPos) {
    return root ? root->updatePoint(oldPos, newPos) : false;
}
