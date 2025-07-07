# Geometría Computacional

Un proyecto de geometría computacional implementado en C++ que incluye clases para puntos, vectores y polígonos con soporte para plantillas (templates).

## Características

- **Point**: Clase template para representar puntos en 2D y 3D
- **Vector**: Clase template para operaciones vectoriales
- **Polygon**: Clase template para polígonos con cálculo de área y orientación

## Estructura del Proyecto

```
.
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   ├── Point/
│   │   ├── Point.h
│   │   └── Point.cpp
│   ├── Vector/
│   │   ├── Vector.h
│   │   └── Vector.cpp
│   └── Poligon/
│       ├── Poligon.h
│       └── Poligon.cpp
└── test/
    ├── CMakeLists.txt
    └── tests.cpp
```

## Requisitos

- CMake 3.14 o superior
- Compilador compatible con C++17
- Google Test (se descarga automáticamente)

## Compilación y Ejecución

### 1. Crear directorio de compilación

```bash
mkdir build
cd build
```

### 2. Configurar con CMake

```bash
cmake ..
```

### 3. Compilar el proyecto

```bash
cmake --build .
```

O alternativamente:

```bash
make
```

### 4. Ejecutar la aplicación principal

```bash
./main_app
```

## Ejecutar Tests

### Opción 1: Usando CTest (Recomendado)

```bash
# Desde el directorio build
ctest
```

Para obtener más información sobre los tests:

```bash
ctest --verbose
```

### Opción 2: Ejecutar directamente el binario de tests

```bash
# Desde el directorio build
./test/run_tests
```

### Opción 3: Ejecutar tests específicos

```bash
# Ejecutar solo tests de Point
./test/run_tests --gtest_filter="PointTest*"

# Ejecutar solo tests de Vector
./test/run_tests --gtest_filter="VectorTest*"

# Ejecutar solo tests de Polygon
./test/run_tests --gtest_filter="PolygonTest*"
```

## Comandos Completos de Ejemplo

```bash
# Clonar y construir el proyecto completo
git clone <repository-url>
cd GeometriaComputacional

# Compilar
mkdir build && cd build
cmake ..
cmake --build .

# Ejecutar aplicación
./main_app

# Ejecutar todos los tests
ctest

# Limpiar y recompilar
cd ..
rm -rf build
mkdir build && cd build
cmake ..
cmake --build .
```

## Configuración de Debug

Para compilar en modo debug:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Configuración de Release

Para compilar en modo release (optimizado):

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Troubleshooting

### Error de Google Test

Si tienes problemas con Google Test, asegúrate de tener conexión a internet ya que CMake descarga automáticamente las dependencias.

### Error de C++17

Si obtienes errores relacionados con C++17, verifica que tu compilador sea compatible:

```bash
# Para GCC
gcc --version  # Debe ser 7.0 o superior

# Para Clang
clang --version  # Debe ser 5.0 o superior
```

### Problemas con CMake

Si CMake no encuentra el compilador:

```bash
# Especificar compilador manualmente
cmake -DCMAKE_CXX_COMPILER=g++ ..
```

## Uso de las Clases

### Point

```cpp
Point<double> p1(1.0, 2.0);        // Punto 2D
Point<double> p2(1.0, 2.0, 3.0);   // Punto 3D
double distance = p1.dist(p2);
```

### Vector

```cpp
Vector<double> v1(3.0, 4.0);
Vector<double> v2(1.0, 2.0);
double dot = v1.dot(v2);
Vector<double> sum = v1 + v2;
```

### Polygon

```cpp
std::vector<Point<double>> vertices = {
    Point<double>(0.0, 0.0),
    Point<double>(1.0, 0.0),
    Point<double>(1.0, 1.0),
    Point<double>(0.0, 1.0)
};
Poligon<double> polygon(vertices);
double area = polygon.area();
bool isCCW = polygon.isCCW();
```
