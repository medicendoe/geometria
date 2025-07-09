#ifndef ACONVEXHULLSTRATEGY_H
#define ACONVEXHULLSTRATEGY_H
#include <vector>
#include "Poligon/Poligon.h"
#include "Point/Point.h"

template<typename T>
class AConvexHullStrategy {
public:
    virtual Poligon<T> apply(const std::vector<Point<T>> cloud) = 0;
};

#endif // ACONVEXHULLSTRATEGY_H
