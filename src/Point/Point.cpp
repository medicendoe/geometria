#include "Point.h"

template <typename T>
Point<T>::Point(const T x, const T y) : Point(x, y, T(0)) {}

template <typename T>
Point<T>::Point(const T x, const T y, const T z) : x(x), y(y), z(z) {}

template <typename T>
Point<T>::Point(const Point<T> &p) : x(p.x), y(p.y), z(p.z) {}

template <typename T>
T Point<T>::dist(const Point<T>& otro) const {
    T dx = x - otro.x;
    T dy = y - otro.y;
    T dz = z - otro.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

template <typename T>
T Point<T>::getX() const {
    return x;
}

template <typename T>
T Point<T>::getY() const {
    return y;
}

template <typename T>
T Point<T>::getZ() const {
    return z;
}

template <typename T>
bool Point<T>::operator==(const Point<T>& otro) const {
    return (x == otro.x && y == otro.y && z == otro.z);
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Point<U>& punto) {
    os << "(" << punto.x << ", " << punto.y << ", " << punto.z << ")";
    return os;
}

// Explicit template instantiations
template class Point<double>;
template class Point<int>;
template class Point<float>;
template std::ostream& operator<<(std::ostream& os, const Point<double>& punto);
template std::ostream& operator<<(std::ostream& os, const Point<int>& punto);
template std::ostream& operator<<(std::ostream& os, const Point<float>& punto);
