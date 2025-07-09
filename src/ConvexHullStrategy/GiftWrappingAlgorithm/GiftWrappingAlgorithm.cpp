#include "Vector/Vector.h"
#include <limits>
#include <cmath>
#include <type_traits>

template<typename T>
Poligon<T> GiftWrappingAlgorithm<T>::apply(const std::vector<Point<T>>& cloud) {
    if (cloud.size() < 3) {
        return Poligon<T>();
    }
    
    std::vector<Point<T>> hull;
    
    size_t startId = 0;
    for (size_t i = 1; i < cloud.size(); i++) {
        if (cloud[i].y < cloud[startId].y) {
            startId = i;
        } else if (cloud[i].y == cloud[startId].y && cloud[i].x < cloud[startId].x) {
            startId = i;
        }
    }

    size_t currentId = startId;
    
    do {
        hull.push_back(cloud[currentId]);
        
        size_t nextId = (current + 1) % cloud.size();
        
        // Find the most counterclockwise point
        for (size_t i = 0; i < cloud.size(); i++) {
            if (orientation(cloud[current], cloud[i], cloud[next]) == 2) {
                next = i;
            }
        }
        
        current = next;
    } while (current != startId);
    
    return Poligon<T>(hull);
}

template<typename T>
T GiftWrappingAlgorithm<T>::orientation(const Point<T>& current, const Point<T>& aspirant, const Point<T>& challenger) const {
    Vector<T> v1 = aspirant - current;
    Vector<T> v2 = challenger - current; 
    Vector<T> cross = v1.cross(v2);
    
    if (isZero(cross.getZ())) return 0;  // collinear
    return (cross.getZ() > 0) ? 1 : 2; // clockwise or counterclockwise
}

// Helper method for flexible zero comparison
template<typename T>
bool GiftWrappingAlgorithm<T>::isZero(T value) const {
    if constexpr (std::is_floating_point_v<T>) {
        return std::abs(value) < std::numeric_limits<T>::epsilon() * 10;
    } else {
        return value == 0;
    }
}

// Explicit template instantiation for common types
template class GiftWrappingAlgorithm<int>;
template class GiftWrappingAlgorithm<float>;
template class GiftWrappingAlgorithm<double>;
