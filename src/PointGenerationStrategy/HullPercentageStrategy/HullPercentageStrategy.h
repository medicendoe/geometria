#ifndef HULLPERCENTAGESTRATEGY_H
#define HULLPERCENTAGESTRATEGY_H

#include "PointGenerationStrategy/APointGenerationStrategy.h"

template<typename T>
class HullPercentageStrategy : public APointGenerationStrategy<T> {
public:
    std::vector<Point<T>> generate(size_t n_points, double percentage) override;
};

#endif
