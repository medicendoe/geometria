#ifndef RANDOMPOINTSTRATEGY_H
#define RANDOMPOINTSTRATEGY_H

#include "PointGenerationStrategy/APointGenerationStrategy.h"

template<typename T>
class RandomPointGenerator : public APointGenerationStrategy<T> {
public:
    std::vector<Point<T>> generate(size_t n_points, double param = 0.0) override;
};

#endif
