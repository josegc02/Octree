//
// Created by esteb on 17/05/2025.
//
#include <iostream>
#include <cassert>
#include "Octree.h"
#include "Point3D.h"

// Test: Insertar puntos y buscar cercanos
void test_insert_and_getNearby() {
    Octree tree({0,0,0}, 10.0, 1.0);

    Point3D p1{1,1,1};
    Point3D p2{-1,-1,-1};
    Point3D p3{5,5,5};
    Point3D p4{0,0,0};

    assert(tree.insert(p1));
    assert(tree.insert(p2));
    assert(tree.insert(p3));
    assert(tree.insert(p4));

    // Buscar puntos cerca del origen con radio 2.0
    auto results = tree.getNearby({0,0,0}, 2.0);

    // Debe contener p1, p2 y p4, pero no p3
    bool found_p1 = false, found_p2 = false, found_p3 = false, found_p4 = false;
    for (const auto& p : results) {
        if (p == p1) found_p1 = true;
        if (p == p2) found_p2 = true;
        if (p == p3) found_p3 = true;
        if (p == p4) found_p4 = true;
    }

    assert(found_p1);
    assert(found_p2);
    assert(!found_p3);
    assert(found_p4);

    std::cout << "[test_insert_and_getNearby] Passed\n";
}

// Test: Remover puntos
void test_remove() {
    Octree tree({0,0,0}, 10.0, 1.0);

    Point3D p1{1,1,1};
    Point3D p2{2,2,2};

    assert(tree.insert(p1));
    assert(tree.insert(p2));

    // Remover p1
    assert(tree.remove(p1));

    // Verificar que p1 no está en el árbol
    auto results = tree.getNearby({0,0,0}, 5.0);
    for (const auto& p : results) {
        assert(!(p == p1));
    }

    // p2 sigue ahí
    bool found_p2 = false;
    for (const auto& p : results) {
        if (p == p2) found_p2 = true;
    }
    assert(found_p2);

    // Intentar remover p1 de nuevo debe fallar
    assert(!tree.remove(p1));

    std::cout << "[test_remove] Passed\n";
}

// Test: Insertar muchos puntos y comprobar tamaño aproximado de resultados
void test_bulk_insert_query() {
    Octree tree({500, 0, 0}, 1000.0, 1.0); // centro y tamaño ajustados

    for (int i = 0; i < 1000; ++i) {
        tree.insert({static_cast<double>(i), 0, 0});
    }

    auto results = tree.getNearby({500, 0, 0}, 10.0);

    //std::cout << "[test_bulk_insert_query] Resultados obtenidos: " << results.size() << std::endl;

    assert(results.size() >= 19 && results.size() <= 23);

    std::cout << "[test_bulk_insert_query] Passed\n";
}

void test_insert_duplicates() {
    Octree tree({0,0,0}, 10.0, 1.0);

    Point3D p{1,1,1};

    assert(tree.insert(p));
    assert(tree.insert(p));  // Insertar duplicado permitido

    auto results = tree.getNearby(p, 0.1);

    int count = 0;
    for (const auto& pt : results) {
        if (pt == p) count++;
    }
    assert(count == 2); // Ambos puntos deberían encontrarse

    std::cout << "[test_insert_duplicates] Passed\n";
}

void test_remove_nonexistent() {
    Octree tree({0,0,0}, 10.0, 1.0);

    Point3D p1{1,1,1};
    Point3D p2{2,2,2};

    assert(tree.insert(p1));
    assert(!tree.remove(p2)); // p2 no está, debe retornar false

    auto results = tree.getNearby(p1, 0.1);
    assert(results.size() == 1);
    assert(results[0] == p1);

    std::cout << "[test_remove_nonexistent] Passed\n";
}

void test_multiple_insert_remove() {
    Octree tree({0,0,0}, 20.0, 1.0);

    std::vector<Point3D> points = {
        {1,1,1}, {2,2,2}, {3,3,3}, {4,4,4}, {5,5,5}
    };

    for (const auto& p : points) {
        assert(tree.insert(p));
    }

    // Remover algunos puntos
    assert(tree.remove(points[1])); // {2,2,2}
    assert(tree.remove(points[3])); // {4,4,4}

    // Buscar todos con radio amplio
    auto results = tree.getNearby({0,0,0}, 10.0);

    // Debe contener los puntos restantes
    assert(results.size() == 3);
    for (const auto& p : points) {
        if (p == points[1] || p == points[3]) continue;
        bool found = false;
        for (const auto& r : results) {
            if (r == p) found = true;
        }
        assert(found);
    }

    std::cout << "[test_multiple_insert_remove] Passed\n";
}

void test_query_empty_tree() {
    Octree tree({0,0,0}, 10.0, 1.0);

    auto results = tree.getNearby({0,0,0}, 5.0);
    assert(results.empty());

    std::cout << "[test_query_empty_tree] Passed\n";
}

void test_points_on_boundaries() {
    double side = 10.0;
    Octree tree({0,0,0}, side, 1.0);
    double half = side / 2;

    Point3D corners[] = {
        {0 + half, 0, 0},
        {0 - half, 0, 0},
        {0, 0 + half, 0},
        {0, 0 - half, 0},
        {0, 0, 0 + half},
        {0, 0, 0 - half}
    };

    for (const auto& p : corners) {
        assert(tree.insert(p));
    }

    // Buscar alrededor del centro con radio mayor a half
    auto results = tree.getNearby({0,0,0}, half + 0.1);
    assert(results.size() == 6);

    std::cout << "[test_points_on_boundaries] Passed\n";
}

void test_end_to_end() {

    Octree tree({0, 0, 0}, 20.0, 1.0);

    Point3D p1{1, 1, 1};
    Point3D p2{5, 5, 5};
    Point3D p3{-1, -1, -1};  // cambiado para estar cerca
    Point3D p4{9, 0, 0};

    assert(tree.insert(p1));
    assert(tree.insert(p2));
    assert(tree.insert(p3));
    assert(tree.insert(p4));

    auto near_origin = tree.getNearby({0, 0, 0}, 3.0);

    assert(near_origin.size() == 2);
    bool found_p1 = false, found_p3 = false;
    for (const auto& p : near_origin) {
        if (p == p1) found_p1 = true;
        if (p == p3) found_p3 = true;
    }
    assert(found_p1 && found_p3);
    // Eliminar p1
    assert(tree.remove(p1));

    // Consultar nuevamente cerca de origen, p1 no debe aparecer
    near_origin = tree.getNearby({0,0,0}, 3.0);
    found_p1 = false; found_p3 = false;
    for (auto& p : near_origin) {
        if (p == p1) found_p1 = true;
        if (p == p3) found_p3 = true;
    }
    assert(!found_p1 && found_p3);

    // Consultar cerca de p4, radio 2 (debería encontrar p4)
    auto near_p4 = tree.getNearby({10,0,0}, 2.0);
    assert(near_p4.size() == 1);
    assert(near_p4[0] == p4);

    std::cout << "["<< __func__ <<"] Passed\n";
}
void test_subdivide_and_merge() {
    Octree tree({0,0,0}, 20.0, 1.0);

    // Insertar más de MAX_POINTS puntos cercanos para forzar subdivisión
    for (int i = 0; i < OctreeNode::MAX_POINTS + 5; ++i) {
        assert(tree.insert({double(i) - 5, 0, 0}));
    }

    // Eliminar casi todos para forzar merge
    for (int i = 0; i < OctreeNode::MAX_POINTS + 4; ++i) {
        assert(tree.remove({double(i) - 5, 0, 0}));
    }

    // Debe quedar solo un punto
    auto results = tree.getNearby({0,0,0}, 100);
    assert(results.size() == 1);

    std::cout << "[test_subdivide_and_merge] Passed\n";
}
