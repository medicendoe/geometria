#ifndef ADIVIDEANDCONQUERALGORITHM_H
#define ADIVIDEANDCONQUERALGORITHM_H

#include "ConvexHullStrategy/AConvexHullStrategy.h"
#include <vector>
#include "Poligon/Poligon.h"
#include "Point/Point.h"
#include "ConvexHullStrategy/Orientation.h"

template<typename T>
class DivideAndConquerAlgorithm : public AConvexHullStrategy<T> {
public:
    Poligon<T> apply(const std::vector<Point<T>>& cloud) override;

private:
    std::vector<Point<T>> solve(const std::vector<Point<T>>& points) const;
    std::vector<Point<T>> merge(const std::vector<Point<T>>& leftHull, std::vector<Point<T>>& rightHull) const;
    Orientation orientation(const Point<T>& current, const Point<T>& aspirant, const Point<T>& challenger) const;
    bool isZero(T value) const;
};

#endif
