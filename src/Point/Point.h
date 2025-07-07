#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <cmath>

template <typename T>
class Point {
    public:
        Point(const T x, const T y);
        Point(const T x, const T y, const T z);
        Point(const Point<T> &p);

        T dist(const Point<T>& otro) const;

        T getX() const;
        T getY() const;
        T getZ() const;

        bool operator==(const Point<T>& otro) const;
        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Point<U>& punto);
    private:
        T x;
        T y;
        T z;
};
#endif