#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include "Point3D.h"
#include "OctreeNode.h"

class Octree {
private:
    OctreeNode* root;
    double minSideLength;

public:
    // Constructor: centro del cubo raíz, longitud lado y tamaño mínimo para subdividir
    Octree(const Point3D& center, double sideLength, double minSide);

    ~Octree();

    // Insertar un punto
    bool insert(const Point3D& p);

    // Eliminar un punto
    bool remove(const Point3D& p);

    // Obtener puntos cercanos a 'position' en radio 'maxDistance'
    std::vector<Point3D> getNearby(const Point3D& position, double maxDistance) const;

    // Opcional: imprimir estructura para debug
    void print() const;
    const OctreeNode* getRoot() const ;

    bool searchPoint(const Point3D& p) const;

    bool updatePoint(const Point3D& oldPos, const Point3D& newPos);
};

#endif // OCTREE_H
