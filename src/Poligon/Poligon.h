#ifndef POLIGON_H
#define POLIGON_H
#include <iostream>
#include <vector>
#include "Point/Point.h"
#include "Vector/Vector.h"
template <typename T>
class Poligon {
    public:
        Poligon(const std::vector<Point<T>>& vertexes);

        size_t numVertexes() const;
        bool isCCW() const;
        T area() const;
        void fromCWToCCW();

        const Point<T>& operator[](size_t index) const;
        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Poligono<U>& p);
    private:
        std::vector<Point<T>> vertexes;
        T shoelace() const;

};
#endif