#include <algorithm>
#include <random>
#include "PointGenerationStrategy/APointGenerationStrategy.h"

#define PI 3.14159265358979323846

template<typename T>
class HullPercentageStrategy : public APointGenerationStrategy<T> {
    public:
        std::vector<Point<T>> generate(size_t n_points, double percentage) override {
            std::vector<Point<T>> cloud;
            cloud.reserve(n_points);

            int k_hull = static_cast<int>(n_points * (percentage / 100.0));
            int n_interior = n_points - k_hull;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distrib_angle(0.0, 2 * PI);
            std::uniform_real_distribution<> distrib_0_1(0.0, 1.0); // Para sqrt y variación

            double cx = 5000.0, cy = 5000.0, R = 4500.0;

            for (int i = 0; i < k_hull; ++i) {
                double theta = distrib_angle(gen);
                cloud.push_back(Point<T>(cx + R * cos(theta), cy + R * sin(theta)));
            }

            for (int i = 0; i < n_interior; ++i) {
                double theta = distrib_angle(gen);
                double r = R * sqrt(distrib_0_1(gen));
                cloud.push_back(Point<T>(cx + r * cos(theta), cy + r * sin(theta)));
            }
            
            std::shuffle(cloud.begin(), cloud.end(), gen);

            return cloud;
        }
};


template class HullPercentageStrategy<double>;
template class HullPercentageStrategy<float>;
template class HullPercentageStrategy<int>;