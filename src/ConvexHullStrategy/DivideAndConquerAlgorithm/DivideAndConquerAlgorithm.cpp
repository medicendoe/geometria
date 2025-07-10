#include "DivideAndConquerAlgorithm.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <type_traits>

template<typename T>
Poligon<T> ADivideAndConquerAlgorithm<T>::apply(const std::vector<Point<T>>& cloud) {
    if (cloud.size() < 3) {
        return Poligon<T>(cloud);
    }
    
    std::vector<Point<T>> sortedPoints = cloud;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Point<T>& a, const Point<T>& b) {
        return a.getX() < b.getX() || (a.getX() == b.getX() && a.getY() < b.getY());
    });
    
    std::vector<Point<T>> hullPoints = solve(sortedPoints);

    return Poligon<T>(hullPoints);
}

template<typename T>
std::vector<Point<T>> ADivideAndConquerAlgorithm<T>::solve(const std::vector<Point<T>>& points) const {
    int n = points.size();
    
    if (n <= 3) {
        return points;
    }
    
    int mid = n / 2;
    std::vector<Point<T>> leftPoints(points.begin(), points.begin() + mid);
    std::vector<Point<T>> rightPoints(points.begin() + mid, points.end());
    
    std::vector<Point<T>> leftHull = solve(leftPoints);
    std::vector<Point<T>> rightHull = solve(rightPoints);
    
    return merge(leftHull, rightHull);
}

template<typename T>
std::vector<Point<T>> ADivideAndConquerAlgorithm<T>::merge(const std::vector<Point<T>>& leftHull, std::vector<Point<T>>& rightHull) const {

    size_t leftStartId = leftHull.size()-1, rightStartId = 0;

    size_t upperLeft = leftStartId, upperRight = rightStartId;
    bool done = false;
    while (!done) {
        done = true;
        while (orientation(rightHull[upperRight], leftHull[upperLeft], 
                        leftHull[(upperLeft - 1) % leftHull.size()]) != Orientation::COUNTERCLOCKWISE) {
            upperLeft = (upperLeft + 1) % leftHull.size();
            done = false;
        }
        while (orientation(leftHull[upperLeft], rightHull[upperRight], 
                        rightHull[(upperRight + 1) % rightHull.size()]) != Orientation::CLOCKWISE) {
            upperRight = (upperRight - 1 + rightHull.size()) % rightHull.size();
            done = false;
        }
    }
    
    size_t lowerLeft = leftStartId, lowerRight = rightStartId;
    done = false;
    while (!done) {
        done = true;
        while (orientation(leftHull[lowerLeft], rightHull[lowerRight], 
                        rightHull[(lowerRight + 1) % rightHull.size()]) != Orientation::COUNTERCLOCKWISE) {
            lowerRight = (lowerRight + 1) % rightHull.size();
            done = false;
        }
        while (orientation(rightHull[lowerRight], leftHull[lowerLeft], 
                        leftHull[(lowerLeft - 1 + leftHull.size()) % leftHull.size()]) != Orientation::CLOCKWISE) {
            lowerLeft = (lowerLeft - 1 + leftHull.size()) % leftHull.size();
            done = false;
        }
    }
    
    // Merge hulls
    std::vector<Point<T>> result;
    size_t curr = upperLeft;
    result.push_back(leftHull[curr]);
    while (curr != lowerLeft) {
        curr = (curr + 1) % leftHull.size();
        result.push_back(leftHull[curr]);
    }
    
    curr = lowerRight;
    result.push_back(rightHull[curr]);
    while (curr != upperRight) {
        curr = (curr + 1) % rightHull.size();
        result.push_back(rightHull[curr]);
    }
    
    return result;
}

template<typename T>
Orientation ADivideAndConquerAlgorithm<T>::orientation(const Point<T>& current, const Point<T>& aspirant, const Point<T>& challenger) const {
    Vector<T> v1 = aspirant - current;
    Vector<T> v2 = challenger - current; 
    Vector<T> cross = v1.cross(v2);
    
    if (isZero(cross.getZ())) return Orientation::COLLINEAR;
    return (cross.getZ() > 0) ? Orientation::CLOCKWISE : Orientation::COUNTERCLOCKWISE;
}

// Helper method for flexible zero comparison
template<typename T>
bool ADivideAndConquerAlgorithm<T>::isZero(T value) const {
    if constexpr (std::is_floating_point_v<T>) {
        return std::abs(value) < std::numeric_limits<T>::epsilon() * 10;
    } else {
        return value == 0;
    }
}

// Explicit template instantiations
template class ADivideAndConquerAlgorithm<int>;
template class ADivideAndConquerAlgorithm<float>;
template class ADivideAndConquerAlgorithm<double>;
