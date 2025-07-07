#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include "Point/Point.h"
template <typename T>
class Vector {
    public:
        Vector(const T x, const T y);
        Vector(const Point<T> end);
        Vector(const Vector<T> &v);

        T cross(const Vector<T>& other) const;
        T dot(const Vector<T>& other) const;
        T length() const;

        T operator+(const Vector<T>& other) const;
        T operator*(const T scalar) const;
        bool operator==(const Vector<T>& other) const;
        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Vector<U>& vector);

    private:
        Point<T> end;
}
#endif