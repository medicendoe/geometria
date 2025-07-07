#include <iostream>
#include <vector>
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Poligon/Poligon.h"

int main() {
    std::cout << "=== Point Class Demo ===" << std::endl;
    
    // Point constructors
    Point<double> p1(1.0, 2.0);
    Point<double> p2(3.0, 4.0, 5.0);
    Point<double> p3(p1);
    
    std::cout << "Point p1: " << p1 << std::endl;
    std::cout << "Point p2: " << p2 << std::endl;
    std::cout << "Point p3 (copy of p1): " << p3 << std::endl;
    
    // Point methods
    std::cout << "p1.getX(): " << p1.getX() << std::endl;
    std::cout << "p1.getY(): " << p1.getY() << std::endl;
    std::cout << "p1.getZ(): " << p1.getZ() << std::endl;
    std::cout << "Distance p1 to p2: " << p1.dist(p2) << std::endl;
    std::cout << "p1 == p3: " << (p1 == p3) << std::endl;
    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    
    std::cout << "\n=== Vector Class Demo ===" << std::endl;
    
    // Vector constructors
    Vector<double> v1(3.0, 4.0);
    Vector<double> v2(p2);
    Vector<double> v3(v1);
    
    std::cout << "Vector v1: " << v1 << std::endl;
    std::cout << "Vector v2: " << v2 << std::endl;
    std::cout << "Vector v3 (copy of v1): " << v3 << std::endl;
    
    // Vector methods
    std::cout << "v1.length(): " << v1.length() << std::endl;
    std::cout << "v1.dot(v2): " << v1.dot(v2) << std::endl;
    std::cout << "v1.getZ(): " << v1.getZ() << std::endl;
    
    Vector<double> v4 = v1 + v2;
    Vector<double> v5 = v1 * 2.0;
    Vector<double> v6 = v1.cross(v2);
    
    std::cout << "v1 + v2: " << v4 << std::endl;
    std::cout << "v1 * 2: " << v5 << std::endl;
    std::cout << "v1.cross: " << v6 << std::endl;
    std::cout << "v1 == v3: " << (v1 == v3) << std::endl;
    
    std::cout << "\n=== Polygon Class Demo ===" << std::endl;
    
    // Create a square polygon
    std::vector<Point<double>> vertices = {
        Point<double>(0.0, 0.0),
        Point<double>(1.0, 0.0),
        Point<double>(1.0, 1.0),
        Point<double>(0.0, 1.0)
    };
    
    Poligon<double> polygon(vertices);
    
    std::cout << "Polygon: " << polygon << std::endl;
    std::cout << "Number of vertices: " << polygon.numVertexes() << std::endl;
    std::cout << "Is counter-clockwise: " << polygon.isCCW() << std::endl;
    std::cout << "Area: " << polygon.area() << std::endl;
    
    // Access vertices
    std::cout << "First vertex: " << polygon[0] << std::endl;
    
    // Convert to CCW if needed
    polygon.fromCWToCCW();
    std::cout << "After ensuring CCW: " << polygon << std::endl;
    std::cout << "Is counter-clockwise now: " << polygon.isCCW() << std::endl;
    
    // Test with triangle
    std::vector<Point<double>> triangle = {
        Point<double>(0.0, 0.0),
        Point<double>(2.0, 0.0),
        Point<double>(1.0, 2.0)
    };
    
    Poligon<double> tri(triangle);
    std::cout << "\nTriangle: " << tri << std::endl;
    std::cout << "Triangle area: " << tri.area() << std::endl;
    
    return 0;
}
