#include "DivideAndConquerAlgorithm.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <type_traits>

template<typename T>
Poligon<T> DivideAndConquerAlgorithm<T>::apply(const std::vector<Point<T>>& cloud) {
    if (cloud.size() < 3) {
        return Poligon<T>(cloud);
    }
    
    std::vector<Point<T>> sortedPoints = cloud;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Point<T>& a, const Point<T>& b) {
        return a.getX() < b.getX() || (a.getX() == b.getX() && a.getY() < b.getY());
    });
    
    std::vector<Point<T>> hullPoints = solve(sortedPoints);
    
    // Ensure the hull is in CCW order
    Poligon<T> result(hullPoints);
    if (!result.isCCW() && result.numVertexes() > 2) {
        result.fromCWToCCW();
    }

    return result;
}

template<typename T>
std::vector<Point<T>> DivideAndConquerAlgorithm<T>::solve(const std::vector<Point<T>>& points) const {
    int n = points.size();
    
    if (n == 1) {
        return points;
    }
    
    if (n == 2) {
        return points;
    }
    
    if (n == 3) {
        // For 3 points, compute actual convex hull
        std::vector<Point<T>> result;
        
        // Check orientation of the three points
        Orientation orient = orientation(points[0], points[1], points[2]);
        
        if (orient == Orientation::COLLINEAR) {
            // If collinear, return the two extreme points
            T dist01 = points[0].dist(points[1]);
            T dist02 = points[0].dist(points[2]);
            T dist12 = points[1].dist(points[2]);
            
            if (dist01 >= dist02 && dist01 >= dist12) {
                result.push_back(points[0]);
                result.push_back(points[1]);
            } else if (dist02 >= dist01 && dist02 >= dist12) {
                result.push_back(points[0]);
                result.push_back(points[2]);
            } else {
                result.push_back(points[1]);
                result.push_back(points[2]);
            }
        } else {
            // If not collinear, return all three points in correct order
            if (orient == Orientation::COUNTERCLOCKWISE) {
                result.push_back(points[0]);
                result.push_back(points[1]);
                result.push_back(points[2]);
            } else {
                result.push_back(points[0]);
                result.push_back(points[2]);
                result.push_back(points[1]);
            }
        }
        return result;
    }
    
    int mid = n / 2;
    std::vector<Point<T>> leftPoints(points.begin(), points.begin() + mid);
    std::vector<Point<T>> rightPoints(points.begin() + mid, points.end());
    
    std::vector<Point<T>> leftHull = solve(leftPoints);
    std::vector<Point<T>> rightHull = solve(rightPoints);
    
    return merge(leftHull, rightHull);
}

template<typename T>
std::vector<Point<T>> DivideAndConquerAlgorithm<T>::merge(const std::vector<Point<T>>& leftHull, std::vector<Point<T>>& rightHull) const {
    // For now, use a simpler approach: combine both hulls and use Graham scan-like approach
    std::vector<Point<T>> combined;
    combined.insert(combined.end(), leftHull.begin(), leftHull.end());
    combined.insert(combined.end(), rightHull.begin(), rightHull.end());
    
    if (combined.size() <= 3) {
        return combined;
    }
    
    // Find the bottom-most point (and leftmost if tie)
    size_t bottom = 0;
    for (size_t i = 1; i < combined.size(); i++) {
        if (combined[i].getY() < combined[bottom].getY() || 
            (combined[i].getY() == combined[bottom].getY() && combined[i].getX() < combined[bottom].getX())) {
            bottom = i;
        }
    }
    std::swap(combined[0], combined[bottom]);
    
    // Sort points by polar angle with respect to bottom point
    Point<T> pivot = combined[0];
    std::sort(combined.begin() + 1, combined.end(), [&](const Point<T>& a, const Point<T>& b) {
        Orientation orient = orientation(pivot, a, b);
        if (orient == Orientation::COLLINEAR) {
            // If collinear, prefer the one closer to pivot
            return pivot.dist(a) < pivot.dist(b);
        }
        return orient == Orientation::COUNTERCLOCKWISE;
    });
    
    // Build convex hull using Graham scan
    std::vector<Point<T>> hull;
    for (const auto& point : combined) {
        while (hull.size() > 1 && 
                orientation(hull[hull.size()-2], hull[hull.size()-1], point) != Orientation::COUNTERCLOCKWISE) {
            hull.pop_back();
        }
        hull.push_back(point);
    }
    
    return hull;
}

template<typename T>
Orientation DivideAndConquerAlgorithm<T>::orientation(const Point<T>& current, const Point<T>& aspirant, const Point<T>& challenger) const {
    Vector<T> v1(aspirant.getX() - current.getX(), aspirant.getY() - current.getY());
    Vector<T> v2(challenger.getX() - current.getX(), challenger.getY() - current.getY());
    Vector<T> cross = v1.cross(v2);
    
    if (isZero(cross.getZ())) return Orientation::COLLINEAR;
    return (cross.getZ() < 0) ? Orientation::CLOCKWISE : Orientation::COUNTERCLOCKWISE;
}

// Helper method for flexible zero comparison
template<typename T>
bool DivideAndConquerAlgorithm<T>::isZero(T value) const {
    if constexpr (std::is_floating_point_v<T>) {
        return std::abs(value) < std::numeric_limits<T>::epsilon() * 10;
    } else {
        return value == 0;
    }
}

// Explicit template instantiations
template class DivideAndConquerAlgorithm<int>;
template class DivideAndConquerAlgorithm<float>;
template class DivideAndConquerAlgorithm<double>;
