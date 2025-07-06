//
// Created by José Guerrero on 3/07/25.
//

#include "OctreeVisualizer.h"
using namespace open3d;

OctreeVisualizer::OctreeVisualizer(const Octree& t) : tree_(t) {}

std::shared_ptr<geometry::LineSet>
OctreeVisualizer::makeWire(const Point3D& c, double side)
{
    double h = side * 0.5;
    Eigen::Vector3d v[8]{
            {c.x-h,c.y-h,c.z-h},{c.x+h,c.y-h,c.z-h},
            {c.x-h,c.y+h,c.z-h},{c.x+h,c.y+h,c.z-h},
            {c.x-h,c.y-h,c.z+h},{c.x+h,c.y-h,c.z+h},
            {c.x-h,c.y+h,c.z+h},{c.x+h,c.y+h,c.z+h}};
    auto ls = std::make_shared<geometry::LineSet>();
    ls->points_ = std::vector<Eigen::Vector3d>(std::begin(v), std::end(v));
    int e[12][2]={{0,1},{1,3},{3,2},{2,0},
                  {4,5},{5,7},{7,6},{6,4},
                  {0,4},{1,5},{2,6},{3,7}};
    for (auto& p:e) ls->lines_.emplace_back(p[0],p[1]);
    return ls;
}

void OctreeVisualizer::collectWire(const OctreeNode* n,
                                   std::vector<std::shared_ptr<const geometry::Geometry>>& out) const
{
    out.push_back(makeWire(n->center, n->sideLength));
    if (n->children[0])
        for (auto* ch: n->children) if (ch) collectWire(ch, out);
}

void OctreeVisualizer::collectPoints(const OctreeNode* n,
                                     std::vector<Eigen::Vector3d>& out) const
{
    for (auto& p: n->points) out.emplace_back(p.x, p.y, p.z);
    if (n->children[0])
        for (auto* ch: n->children) if (ch) collectPoints(ch, out);
}

std::vector<std::shared_ptr<const geometry::Geometry>>
OctreeVisualizer::asGeometries(bool draw_points, bool draw_wire) const
{
    std::vector<std::shared_ptr<const geometry::Geometry>> geoms;
    if (draw_wire)  collectWire(tree_.getRoot(), geoms);

    if (draw_points) {
        std::vector<Eigen::Vector3d> pts;
        collectPoints(tree_.getRoot(), pts);
        auto pcd = std::make_shared<geometry::PointCloud>();
        pcd->points_ = std::move(pts);
        pcd->PaintUniformColor({0,1,0});
        geoms.push_back(pcd);
    }
    return geoms;
}

void OctreeVisualizer::draw(bool draw_points, bool draw_wire,
                            const std::string& title) const
{
    visualization::DrawGeometries(
            asGeometries(draw_points, draw_wire), title);
}
