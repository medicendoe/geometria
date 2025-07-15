#ifndef APOINTGENERATIONSTRATEGY_H
#define APOINTGENERATIONSTRATEGY_H
#include <vector>
#include "Point/Point.h"

template<typename T>
class APointGenerationStrategy {
public:
    virtual ~APointGenerationStrategy() = default;

    virtual std::vector<Point<T>> generate(size_t n_points, double param = 0.0) = 0;
};
#endif