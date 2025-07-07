#include "Poligon.h"
#include "Vector/Vector.h"
#include <algorithm>
#include <cmath>

template <typename T>
Poligon<T>::Poligon(const std::vector<Point<T>>& vertexes) : vertexes(vertexes) {}

template <typename T>
size_t Poligon<T>::numVertexes() const {
    return static_cast<size_t>(vertexes.size());
}

template <typename T>
T Poligon<T>::shoelace() const {
    if (vertexes.size() < 3) return T(0);
    
    T sum = T(0);
    for (size_t i = 0; i < vertexes.size(); ++i) {
        size_t next = (i + 1) % vertexes.size();
        
        // Create vectors from current vertex to previous and next
        Vector<T> v1(vertexes[i]); 
        Vector<T> v2(vertexes[next]);
        
        // Use cross product from Vector class and extract z coordinate
        Vector<T> crossResult = v1.cross(v2);
        sum += crossResult.getZ();
    }

    return sum;

}

template <typename T>
bool Poligon<T>::isCCW() const {
    return shoelace() > T(0);
}

template <typename T>
T Poligon<T>::area() const {
    return std::abs(shoelace()) / T(2);
}

template <typename T>
void Poligon<T>::fromCWToCCW() {
    if (!isCCW()) {
        std::reverse(vertexes.begin(), vertexes.end());
    }
}

template <typename T>
const Point<T>& Poligon<T>::operator[](size_t index) const {
    return vertexes[index];
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Poligon<U>& p) {
    os << "[";

    for (size_t i = 0; i < p.vertexes.size(); ++i) {
        
        os << p.vertexes[i]; 
        
        if (i < p.vertexes.size() - 1) {
            os << ", ";
        }
    }

    os << "]";
    
    return os;
}

// Explicit template instantiations
template class Poligon<int>;
template class Poligon<float>;
template class Poligon<double>;
template std::ostream& operator<<(std::ostream& os, const Poligon<int>& p);
template std::ostream& operator<<(std::ostream& os, const Poligon<float>& p);
template std::ostream& operator<<(std::ostream& os, const Poligon<double>& p);
