# Documentación del Proyecto Octree - Algortimos y Estructura de datos - UTEC
## Profesor
- Brenner Humberto Ojeda

## Descripción General
Este proyecto pretende dar una introducción a las capacidades del árbol octal a partir de una implementación rigurosa pero básica, pues no soporta el uso genérico. Además, se hará uso de la librería Open3D para poder visualizar la estructura de datos y por último una breve documentación de los métodos de la clase OctreeNode.

## Instalación de Open3D

### MacOS
- 1) Instalar Open3D (Homebrew)
brew install open3d          # necesita Homebrew ≥ 3.0

- 2) Clonar el proyecto
git clone https://github.com/USER/octree-demo.git
cd octree-demo

- 3) Construir
cmake -B build -DOpen3D_ROOT=$(brew --prefix open3d)
cmake --build build -j
./build/octree                # ← ejecutable

### Linux/GNU
- 1) Descargar y extraer Open3D (C++)
wget https://github.com/isl-org/Open3D/releases/download/v0.19.0/open3d-linux-x86_64-0.19.0.tar.xz
mkdir -p $HOME/opt && tar -xf open3d-linux-x86_64-0.19.0.tar.xz -C $HOME/opt
export Open3D_ROOT=$HOME/opt/Open3D

- 2) Clonar y compilar tu proyecto
git clone https://github.com/USER/octree.git
cd octree
cmake -B build -DOpen3D_ROOT=$Open3D_ROOT
cmake --build build -j$(nproc)
./build/octree

### Windows
- 1) Descargar paquete Release pre-compilado
Invoke-WebRequest -Uri https://github.com/isl-org/Open3D/releases/download/v0.19.0/open3d-windows-x86_64-msvc-Release-0.19.0.zip -OutFile open3d.zip
Expand-Archive open3d.zip C:\libs\Open3D

- 2) Clonar tu proyecto
git clone https://github.com/USER/octree-demo.git
cd octree-demo

- 3) Generar solución y compilar (modo Release)
cmake -B build -A x64 -DOpen3D_ROOT=C:\libs\Open3D
cmake --build build --config Release

- 4) Ejecutar
.\build\Release\octree.exe

## Estructura de implementación
Creamos 3 clases: OctreeNode, Octree y Point3D.

## Octree


## Documentación
La clase OctreeNode es el que contiene la funcionalidad de la estructura. La clase Octree se podría entender como un wrapper, pues sus métodos solo llaman a los metodos del root. Por lo tanto, la documentación es sobre los métodos de OctreeNode.

### Contains
- Recibe un objeto de la clase Point3D. Le preguntará al nodo. 
### Subdivide

### CollectAllPoints

### Merge

### Insert

### Remove

### GetNearby








