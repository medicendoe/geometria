#ifndef GIFTWRAPPINGALGORITHM_H
#define GIFTWRAPPINGALGORITHM_H

#include "ConvexHullStrategy/AConvexHullStrategy.h"
#include <vector>
#include "Poligon/Poligon.h"
#include "Point/Point.h"

template<typename T>
class GiftWrappingAlgorithm : public AConvexHullStrategy<T> {
public:
    Poligon<T> apply(const std::vector<Point<T>> cloud) override;

private:
    T orientation(const Point<T>& current, const Point<T>& aspirant, const Point<T>& challenger) const;
    bool isZero(T value) const;
};

#endif
