//#include "Octree.h"            // tu estructura pura
#include "OctreeVisualizer.h"  // capa de presentación (usa Open3D)
//
//#include <random>


void insertando_randoms(){
    //------------------------------------------------------------
    // 1. Crea el octree: centro (0,0,0), lado raíz 10, subdivide
    //    mientras los cubos hijos tengan lado ≥ 0.2
    //------------------------------------------------------------
    Octree tree({0, 0, 0}, 10.0, 0.2);     // ctor: (center, side, minSide) :contentReference[oaicite:2]{index=2}

    //------------------------------------------------------------
    // 2. Inserta 100 puntos aleatorios dentro del cubo raíz
    //------------------------------------------------------------
    std::mt19937 gen{1337};
    std::uniform_real_distribution<double> dist(-9.5, 9.5);


    tree.insert({5, 3 ,1});
    for (int i = 0; i < 1000; ++i)
        tree.insert({dist(gen), dist(gen), dist(gen)});
    tree.remove({5, 3 ,1});
    //------------------------------------------------------------
    // 3. Visualiza: wireframe de todos los nodos + puntos verdes
    //------------------------------------------------------------



    OctreeVisualizer vis(tree);
    vis.draw(/*draw_points = */true,
            /*draw_wire   = */true,
                               "Octree con 100 puntos");

}

//-----------------------------------------------------------
// Programa de demo
//-----------------------------------------------------------
int main() {
    insertando_randoms();
    return 0;
}

