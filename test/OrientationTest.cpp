#include <gtest/gtest.h>
#include "Point/Point.h"
#include "ConvexHullStrategy/Orientation.h"

class OrientationTest : public ::testing::Test {
protected:
    Point<double> origin{0.0, 0.0};
    Point<double> right{1.0, 0.0};
    Point<double> up{0.0, 1.0};
    Point<double> upRight{1.0, 1.0};
};

TEST_F(OrientationTest, OrientationValues) {
    EXPECT_EQ(static_cast<int>(Orientation::COLLINEAR), 0);
    EXPECT_EQ(static_cast<int>(Orientation::CLOCKWISE), 1);
    EXPECT_EQ(static_cast<int>(Orientation::COUNTERCLOCKWISE), 2);
}
