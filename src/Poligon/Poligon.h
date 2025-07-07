#ifndef POLIGON_H
#define POLIGON_H
#include <iostream>
#include <vector>
#include "Point/Point.h"
template <typename T>
class Poligon {
    public:
        Poligon(const std::vector<Point<T>>& vertexes);

        int numVertexes() const;
        bool isCCW() const;
        T area() const;
        void fromCWToCCW();

        const Point<T>& operator[](size_t index) const;
    private:
        std::vector<Point<T>> vertexes;
}
#endif