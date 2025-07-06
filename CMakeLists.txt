cmake_minimum_required(VERSION 3.23)
project(CppOctree LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# ─── Rutas a Open3D y libomp ─────────────────────────────────────────────
set(Open3D_ROOT "/Users/joseguerrero/Downloads/open3d-devel-darwin-x86_64-0.19.0")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/libomp")

find_package(OpenMP REQUIRED)
find_package(Open3D REQUIRED)

# ─── Ejecutable + fuentes ────────────────────────────────────────────────
add_executable(CppOctree
        main.cpp
        Octree.cpp  Octree.h
        OctreeNode.cpp  OctreeNode.h
        Point3D.h
        OctreeVisualizer.cpp  OctreeVisualizer.h
        tests.h)

target_link_libraries(CppOctree
        PRIVATE
        Open3D::Open3D
        OpenMP::OpenMP_CXX)

# ─── RPATH para que al ejecutar encuentre libOpen3D y libomp ─────────────
set_target_properties(CppOctree PROPERTIES
        INSTALL_RPATH "@executable_path/../lib;/usr/local/opt/libomp/lib")
