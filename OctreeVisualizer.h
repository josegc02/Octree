//
// Created by José Guerrero on 3/07/25.
//

#ifndef CPPOCTREE_OCTREEVISUALIZER_H
#define CPPOCTREE_OCTREEVISUALIZER_H

#pragma once
#include <open3d/Open3D.h>
#include "Octree.h"
#include <memory>
#include <vector>

class OctreeVisualizer {
public:
    explicit OctreeVisualizer(const Octree& tree);

    /// Abre ventana interactiva con wireframe + puntos
    void draw(bool draw_points = true,
              bool draw_wire   = true,
              const std::string& title = "Octree viewer") const;

    /// Convierte el octree a geometrías Open3D
    std::vector<std::shared_ptr<const open3d::geometry::Geometry>>
    asGeometries(bool draw_points = true, bool draw_wire = true) const;

private:
    const Octree& tree_;

    // helpers internos
    static std::shared_ptr<open3d::geometry::LineSet>
    makeWire(const Point3D& c, double side);

    void collectWire(const OctreeNode* n,
                     std::vector<std::shared_ptr<const open3d::geometry::Geometry>>& out) const;

    void collectPoints(const OctreeNode* n,
                       std::vector<Eigen::Vector3d>& out) const;
};


#endif //CPPOCTREE_OCTREEVISUALIZER_H
