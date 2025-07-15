#include "GiftWrappingAlgorithm.h"
#include "Vector/Vector.h"
#include <limits>
#include <cmath>
#include <type_traits>

template<typename T>
Poligon<T> GiftWrappingAlgorithm<T>::apply(const std::vector<Point<T>>& cloud) {
    if (cloud.size() <= 3) {
        return Poligon<T>(cloud);
    }

    std::vector<Point<T>> sortedPoints = cloud;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Point<T>& a, const Point<T>& b) {
        return a.getX() < b.getX() || (a.getX() == b.getX() && a.getY() < b.getY());
    });
    
    std::vector<Point<T>> hull;
    
    size_t currentId = 0;
    
    do {
        hull.push_back(sortedPoints[currentId]);
        
        size_t nextId = (currentId + 1) % sortedPoints.size();
        
        // Find the most counterclockwise point
        for (size_t i = 0; i < sortedPoints.size(); i++) {
            switch (orientation(sortedPoints[currentId], sortedPoints[nextId], sortedPoints[i])) {
                case Orientation::COUNTERCLOCKWISE:
                    nextId = i;
                    break;
                case Orientation::COLLINEAR:
                    if (sortedPoints[currentId].dist(sortedPoints[i]) > sortedPoints[currentId].dist(sortedPoints[nextId])) {
                        nextId = i;
                    }
                    break;
                case Orientation::CLOCKWISE:
                    break;
            }
        }
        
        currentId = nextId;
    } while (currentId != 0);
    
    return Poligon<T>(hull);
}

template<typename T>
Orientation GiftWrappingAlgorithm<T>::orientation(const Point<T>& current, const Point<T>& aspirant, const Point<T>& challenger) const {
    Vector<T> v1(aspirant.getX() - current.getX(), aspirant.getY() - current.getY());
    Vector<T> v2(challenger.getX() - current.getX(), challenger.getY() - current.getY());
    Vector<T> cross = v1.cross(v2);
    
    if (isZero(cross.getZ())) return Orientation::COLLINEAR;
    return (cross.getZ() < 0) ? Orientation::CLOCKWISE : Orientation::COUNTERCLOCKWISE;
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
