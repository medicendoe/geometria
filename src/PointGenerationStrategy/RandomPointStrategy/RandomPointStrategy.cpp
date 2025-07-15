#include <random>
#include "PointGenerationStrategy/APointGenerationStrategy.h"

template<typename T>
class RandomPointGenerator : public APointGenerationStrategy<T> {
    public:
        std::vector<Point<T>> generate(size_t n_points, double param = 0.0) override {
            std::vector<Point<T>> cloud;
            cloud.reserve(n_points);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distrib(0.0, 10000.0);

            for (int i = 0; i < n_points; ++i) {
                T x = distrib(gen);
                T y = distrib(gen);
                cloud.push_back(Point<T>(x, y));
            }

            return cloud;
        }
};

template class RandomPointGenerator<double>;
template class RandomPointGenerator<float>;
template class RandomPointGenerator<int>;