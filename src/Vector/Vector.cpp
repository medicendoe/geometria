#include "Vector.h"
#include <cmath>

template <typename T>
Vector<T>::Vector(const T x, const T y) : end(x, y) {}

template <typename T>
Vector<T>::Vector(const Point<T> end) : end(end) {}

template <typename T>
Vector<T>::Vector(const Vector<T> &v) : end(v.end) {}

template <typename T>
Vector<T> Vector<T>::cross(const Vector<T>& other) const {
    T crossProduct = end.getX() * other.end.getY() - end.getY() * other.end.getX();
    return Vector<T>(0, 0, crossProduct);
}

template <typename T>
T Vector<T>::cross2D(const Vector<T>& other) const {
    return end.getX() * other.end.getY() - end.getY() * other.end.getX();
}

template <typename T>
T Vector<T>::dot(const Vector<T>& other) const {
    return end.getX() * other.end.getX() + end.getY() * other.end.getY();
}

template <typename T>
T Vector<T>::length() const {
    return std::sqrt(dot(*this));
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& other) const {
    return Vector<T>(end.getX() + other.end.getX(), end.getY() + other.end.getY());
}

template <typename T>
Vector<T> Vector<T>::operator*(const T scalar) const {
    return Vector<T>(end.getX() * scalar, end.getY() * scalar);
}

template <typename T>
bool Vector<T>::operator==(const Vector<T>& other) const {
    return end == other.end;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Vector<U>& vector) {
    os << "Vector(" << vector.end.getX() << ", " << vector.end.getY() << ")";
    return os;
}

// Explicit template instantiations
template class Vector<int>;
template class Vector<float>;
template class Vector<double>;
template std::ostream& operator<<(std::ostream&, const Vector<int>&);
template std::ostream& operator<<(std::ostream&, const Vector<float>&);
template std::ostream& operator<<(std::ostream&, const Vector<double>&);
