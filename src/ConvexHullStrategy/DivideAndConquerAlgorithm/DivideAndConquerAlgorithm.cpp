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
        return a.getX() < b.getX() || (a.getX() == b.getX() && a.getY() > b.getY());
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

        bool foundLeft = false;
        while (!foundLeft) {
            switch (orientation(rightHull[upperRight], leftHull[upperLeft], 
                                leftHull[(upperLeft + 1) % leftHull.size()])) {
                case Orientation::COUNTERCLOCKWISE:
                    foundLeft = true;
                    break;
                case Orientation::COLLINEAR:
                    if (leftHull[upperLeft].dist(rightHull[upperRight]) < 
                        leftHull[(upperLeft + 1) % leftHull.size()].dist(rightHull[upperRight])) {
                        done = false;
                        upperLeft = (upperLeft + 1) % leftHull.size();
                    } else {
                        foundLeft = true;
                    }
                    break;
                case Orientation::CLOCKWISE:
                    upperLeft = (upperLeft + 1) % leftHull.size();
                    done = false;
                    break;
            }
        }

        bool foundRight = false;
        while (!foundRight) {
            switch (orientation(leftHull[upperLeft], rightHull[upperRight], 
                                rightHull[(upperRight - 1 + rightHull.size()) % rightHull.size()])) {
                case Orientation::COUNTERCLOCKWISE:
                    upperRight = (upperRight - 1 + rightHull.size()) % rightHull.size();
                    done = false;
                    break;
                case Orientation::COLLINEAR:
                    if (rightHull[upperRight].dist(leftHull[upperLeft]) < 
                        rightHull[(upperRight - 1 + rightHull.size()) % rightHull.size()].dist(leftHull[upperLeft])) {
                        done = false;
                        upperRight = (upperRight - 1 + rightHull.size()) % rightHull.size();
                    } else {
                        foundRight = true;
                    }
                    break;
                case Orientation::CLOCKWISE:
                    foundRight = true;
                    break;
            }
        }
    }
    
    size_t lowerLeft = leftStartId, lowerRight = rightStartId;
    done = false;
    while (!done) {
        done = true;

        bool foundRight = false;
        while (!foundRight) {
            switch (orientation(leftHull[lowerLeft], rightHull[lowerRight], 
                                rightHull[(lowerRight + 1) % rightHull.size()])) {
                case Orientation::COUNTERCLOCKWISE:
                    foundRight = true;
                    break;
                case Orientation::COLLINEAR:
                    if (rightHull[lowerRight].dist(leftHull[lowerLeft]) < 
                        rightHull[(lowerRight + 1) % rightHull.size()].dist(leftHull[lowerLeft])) {
                        done = false;
                        lowerRight = (lowerRight + 1) % rightHull.size();
                    } else {
                        foundRight = true;
                    }
                    break;
                case Orientation::CLOCKWISE:
                    lowerRight = (lowerRight + 1) % rightHull.size();
                    done = false;
                    break;
            }
        }

        bool foundLeft = false;
        while (!foundLeft) {
            switch (orientation(rightHull[lowerRight], leftHull[lowerLeft], 
                                leftHull[(lowerLeft - 1 + leftHull.size()) % leftHull.size()])) {
                case Orientation::COUNTERCLOCKWISE:
                    lowerLeft = (lowerLeft - 1 + leftHull.size()) % leftHull.size();
                    done = false;
                    break;
                case Orientation::COLLINEAR:
                    if (leftHull[lowerLeft].dist(rightHull[lowerRight]) < 
                        leftHull[(lowerLeft - 1 + leftHull.size()) % leftHull.size()].dist(rightHull[lowerRight])) {
                        done = false;
                        lowerLeft = (lowerLeft - 1 + leftHull.size()) % leftHull.size();
                    } else {
                        foundLeft = true;
                    }
                    break;
                case Orientation::CLOCKWISE:
                    foundLeft = true;
                    break;
            }
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
    Vector<T> v1(aspirant.getX() - current.getX(), aspirant.getY() - current.getY());
    Vector<T> v2(challenger.getX() - current.getX(), challenger.getY() - current.getY());
    Vector<T> cross = v1.cross(v2);
    
    if (isZero(cross.getZ())) return Orientation::COLLINEAR;
    return (cross.getZ() < 0) ? Orientation::CLOCKWISE : Orientation::COUNTERCLOCKWISE;
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
