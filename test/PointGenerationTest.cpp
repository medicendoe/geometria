#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include "Point/Point.h"
#include "PointGenerationStrategy/APointGenerationStrategy.h"
#include "PointGenerationStrategy/RandomPointStrategy/RandomPointStrategy.h"
#include "PointGenerationStrategy/HullPercentageStrategy/HullPercentageStrategy.h"

class RandomPointGeneratorTest : public ::testing::Test {
protected:
    RandomPointGenerator<double> generator_double;
    RandomPointGenerator<float> generator_float;
    RandomPointGenerator<int> generator_int;
};

TEST_F(RandomPointGeneratorTest, GenerateCorrectNumberOfPoints) {
    auto points = generator_double.generate(100);
    EXPECT_EQ(points.size(), 100);
    
    auto points_small = generator_double.generate(5);
    EXPECT_EQ(points_small.size(), 5);
}

TEST_F(RandomPointGeneratorTest, GenerateZeroPoints) {
    auto points = generator_double.generate(0);
    EXPECT_EQ(points.size(), 0);
    EXPECT_TRUE(points.empty());
}

TEST_F(RandomPointGeneratorTest, GenerateOnePoint) {
    auto points = generator_double.generate(1);
    EXPECT_EQ(points.size(), 1);
    
    // Check that point is within expected range [0, 10000]
    EXPECT_GE(points[0].getX(), 0.0);
    EXPECT_LE(points[0].getX(), 10000.0);
    EXPECT_GE(points[0].getY(), 0.0);
    EXPECT_LE(points[0].getY(), 10000.0);
    EXPECT_EQ(points[0].getZ(), 0.0);
}

TEST_F(RandomPointGeneratorTest, PointsWithinRange) {
    auto points = generator_double.generate(50);
    
    for (const auto& point : points) {
        EXPECT_GE(point.getX(), 0.0);
        EXPECT_LE(point.getX(), 10000.0);
        EXPECT_GE(point.getY(), 0.0);
        EXPECT_LE(point.getY(), 10000.0);
        EXPECT_EQ(point.getZ(), 0.0);
    }
}

TEST_F(RandomPointGeneratorTest, GenerateWithParameter) {
    // Parameter should be ignored for RandomPointGenerator
    auto points1 = generator_double.generate(10, 50.0);
    auto points2 = generator_double.generate(10, 0.0);
    
    EXPECT_EQ(points1.size(), 10);
    EXPECT_EQ(points2.size(), 10);
}

TEST_F(RandomPointGeneratorTest, DifferentNumericTypes) {
    auto points_double = generator_double.generate(10);
    auto points_float = generator_float.generate(10);
    auto points_int = generator_int.generate(10);
    
    EXPECT_EQ(points_double.size(), 10);
    EXPECT_EQ(points_float.size(), 10);
    EXPECT_EQ(points_int.size(), 10);
    
    // Check integer points are actually integers
    for (const auto& point : points_int) {
        EXPECT_EQ(point.getX(), static_cast<int>(point.getX()));
        EXPECT_EQ(point.getY(), static_cast<int>(point.getY()));
    }
}

TEST_F(RandomPointGeneratorTest, Randomness) {
    auto points1 = generator_double.generate(20);
    auto points2 = generator_double.generate(20);
    
    // Check that not all points are identical (very unlikely with random generation)
    bool found_different = false;
    for (size_t i = 0; i < points1.size(); ++i) {
        if (!(points1[i] == points2[i])) {
            found_different = true;
            break;
        }
    }
    EXPECT_TRUE(found_different);
}

class HullPercentageStrategyTest : public ::testing::Test {
protected:
    HullPercentageStrategy<double> generator_double;
    HullPercentageStrategy<float> generator_float;
    HullPercentageStrategy<int> generator_int;
    
    const double EPSILON = 1e-6;
    const double CENTER_X = 5000.0;
    const double CENTER_Y = 5000.0;
    const double RADIUS = 4500.0;
    
    double distanceFromCenter(const Point<double>& point) {
        return std::sqrt((point.getX() - CENTER_X) * (point.getX() - CENTER_X) +
                        (point.getY() - CENTER_Y) * (point.getY() - CENTER_Y));
    }
};

TEST_F(HullPercentageStrategyTest, GenerateCorrectNumberOfPoints) {
    auto points = generator_double.generate(100, 20.0);
    EXPECT_EQ(points.size(), 100);
    
    auto points_small = generator_double.generate(5, 40.0);
    EXPECT_EQ(points_small.size(), 5);
}

TEST_F(HullPercentageStrategyTest, GenerateZeroPoints) {
    auto points = generator_double.generate(0, 50.0);
    EXPECT_EQ(points.size(), 0);
    EXPECT_TRUE(points.empty());
}

TEST_F(HullPercentageStrategyTest, ValidPercentage20) {
    const size_t total_points = 100;
    const double percentage = 20.0;
    auto points = generator_double.generate(total_points, percentage);
    
    EXPECT_EQ(points.size(), total_points);
    
    // Count points on hull (approximately on circle boundary)
    int hull_points = 0;
    int interior_points = 0;
    
    for (const auto& point : points) {
        double dist = distanceFromCenter(point);
        if (std::abs(dist - RADIUS) < 100.0) {  // Tolerance for hull points
            hull_points++;
        } else if (dist < RADIUS - 100.0) {
            interior_points++;
        }
    }
    
    // Should have approximately 20 hull points and 80 interior points
    EXPECT_NEAR(hull_points, 20, 5);  // Allow some tolerance
    EXPECT_NEAR(interior_points, 80, 5);
}

TEST_F(HullPercentageStrategyTest, ValidPercentage50) {
    const size_t total_points = 50;
    const double percentage = 50.0;
    auto points = generator_double.generate(total_points, percentage);
    
    EXPECT_EQ(points.size(), total_points);
    
    int hull_points = 0;
    int interior_points = 0;
    
    for (const auto& point : points) {
        double dist = distanceFromCenter(point);
        if (std::abs(dist - RADIUS) < 100.0) {
            hull_points++;
        } else if (dist < RADIUS - 100.0) {
            interior_points++;
        }
    }
    
    // Should have approximately 25 hull points and 25 interior points
    EXPECT_NEAR(hull_points, 25, 5);
    EXPECT_NEAR(interior_points, 25, 5);
}

TEST_F(HullPercentageStrategyTest, Percentage100) {
    const size_t total_points = 20;
    const double percentage = 100.0;
    auto points = generator_double.generate(total_points, percentage);
    
    EXPECT_EQ(points.size(), total_points);
    
    // All points should be on the hull (boundary)
    for (const auto& point : points) {
        double dist = distanceFromCenter(point);
        EXPECT_NEAR(dist, RADIUS, 100.0);  // All points should be near the circle boundary
    }
}

TEST_F(HullPercentageStrategyTest, Percentage0) {
    const size_t total_points = 30;
    const double percentage = 0.0;
    auto points = generator_double.generate(total_points, percentage);
    
    EXPECT_EQ(points.size(), total_points);
    
    // All points should be interior
    for (const auto& point : points) {
        double dist = distanceFromCenter(point);
        EXPECT_LT(dist, RADIUS - 50.0);  // All points should be well inside the circle
    }
}

TEST_F(HullPercentageStrategyTest, PointsWithinCircle) {
    auto points = generator_double.generate(50, 30.0);
    
    for (const auto& point : points) {
        double dist = distanceFromCenter(point);
        EXPECT_LE(dist, RADIUS + EPSILON);  // All points should be within or on the circle
        EXPECT_EQ(point.getZ(), 0.0);
    }
}

TEST_F(HullPercentageStrategyTest, DifferentNumericTypes) {
    auto points_double = generator_double.generate(10, 20.0);
    auto points_float = generator_float.generate(10, 20.0);
    auto points_int = generator_int.generate(10, 20.0);
    
    EXPECT_EQ(points_double.size(), 10);
    EXPECT_EQ(points_float.size(), 10);
    EXPECT_EQ(points_int.size(), 10);
    
    // Check integer points are actually integers
    for (const auto& point : points_int) {
        EXPECT_EQ(point.getX(), static_cast<int>(point.getX()));
        EXPECT_EQ(point.getY(), static_cast<int>(point.getY()));
    }
}

TEST_F(HullPercentageStrategyTest, Randomness) {
    auto points1 = generator_double.generate(20, 25.0);
    auto points2 = generator_double.generate(20, 25.0);
    
    // Check that not all points are identical
    bool found_different = false;
    for (size_t i = 0; i < points1.size(); ++i) {
        if (!(points1[i] == points2[i])) {
            found_different = true;
            break;
        }
    }
    EXPECT_TRUE(found_different);
}

TEST_F(HullPercentageStrategyTest, ShuffleProperty) {
    // Generate points multiple times and check that order changes
    auto points1 = generator_double.generate(10, 50.0);
    auto points2 = generator_double.generate(10, 50.0);
    
    // Points should be in different order (very unlikely to be identical after shuffle)
    bool order_different = false;
    for (size_t i = 0; i < points1.size(); ++i) {
        if (!(points1[i] == points2[i])) {
            order_different = true;
            break;
        }
    }
    EXPECT_TRUE(order_different);
}

TEST_F(HullPercentageStrategyTest, EdgeCaseOnePoint) {
    auto points = generator_double.generate(1, 100.0);
    EXPECT_EQ(points.size(), 1);
    
    double dist = distanceFromCenter(points[0]);
    EXPECT_NEAR(dist, RADIUS, 100.0);
}

TEST_F(HullPercentageStrategyTest, InteriorPointDistribution) {
    const size_t total_points = 1000;
    const double percentage = 10.0;  // Only 10% on hull, 90% interior
    auto points = generator_double.generate(total_points, percentage);
    
    int interior_count = 0;
    for (const auto& point : points) {
        double dist = distanceFromCenter(point);
        if (dist < RADIUS - 100.0) {
            interior_count++;
        }
    }
    
    // Should have approximately 900 interior points
    EXPECT_NEAR(interior_count, 900, 50);
}

class PointGenerationStrategyPolymorphismTest : public ::testing::Test {
protected:
    std::unique_ptr<APointGenerationStrategy<double>> random_strategy;
    std::unique_ptr<APointGenerationStrategy<double>> hull_strategy;
    
    void SetUp() override {
        random_strategy = std::make_unique<RandomPointGenerator<double>>();
        hull_strategy = std::make_unique<HullPercentageStrategy<double>>();
    }
};

TEST_F(PointGenerationStrategyPolymorphismTest, PolymorphicUsage) {
    auto random_points = random_strategy->generate(50);
    auto hull_points = hull_strategy->generate(50, 30.0);
    
    EXPECT_EQ(random_points.size(), 50);
    EXPECT_EQ(hull_points.size(), 50);
}

TEST_F(PointGenerationStrategyPolymorphismTest, VirtualDestructor) {
    // Test that virtual destructor works correctly
    APointGenerationStrategy<double>* strategy = new RandomPointGenerator<double>();
    delete strategy;  // Should not cause issues with virtual destructor
    
    strategy = new HullPercentageStrategy<double>();
    delete strategy;
    
    // If we reach here without crashes, virtual destructor is working
    SUCCEED();
}
