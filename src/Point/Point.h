#ifndef POINT_H
#define POINT_H
#include <iostream>
template <typename T>
class Point {
    public:
        Point(const T x, const T y);
        Point(const Point<T> &p);

        T dist(const Punto<T>& otro) const;

        bool operator==(const Punto<T>& otro) const;
        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Punto<U>& punto);
    private:
        T x;
        T y;
        T z;
        Point(const T x, const T y, const T z): x(x), y(y), z(z) {};
}
#endif