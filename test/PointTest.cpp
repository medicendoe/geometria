#include <gtest/gtest.h>
#include <sstream>
#include "Point/Point.h"

class PointTest : public ::testing::Test {
protected:
    Point<double> p1{1.0, 2.0};
    Point<double> p2{3.0, 4.0, 5.0};
    Point<int> pi{1, 2};
};

TEST_F(PointTest, Constructor2D) {
    EXPECT_DOUBLE_EQ(p1.getX(), 1.0);
    EXPECT_DOUBLE_EQ(p1.getY(), 2.0);
    EXPECT_DOUBLE_EQ(p1.getZ(), 0.0);
}

TEST_F(PointTest, Constructor3D) {
    EXPECT_DOUBLE_EQ(p2.getX(), 3.0);
    EXPECT_DOUBLE_EQ(p2.getY(), 4.0);
    EXPECT_DOUBLE_EQ(p2.getZ(), 5.0);
}

TEST_F(PointTest, CopyConstructor) {
    Point<double> p3(p1);
    EXPECT_DOUBLE_EQ(p3.getX(), 1.0);
    EXPECT_DOUBLE_EQ(p3.getY(), 2.0);
    EXPECT_DOUBLE_EQ(p3.getZ(), 0.0);
}

TEST_F(PointTest, Distance) {
    Point<double> origin(0.0, 0.0);
    Point<double> point(3.0, 4.0);
    EXPECT_DOUBLE_EQ(origin.dist(point), 5.0);
}

TEST_F(PointTest, EqualityOperator) {
    Point<double> p3(1.0, 2.0);
    EXPECT_TRUE(p1 == p3);
    EXPECT_FALSE(p1 == p2);
}

TEST_F(PointTest, OutputOperator) {
    std::ostringstream oss;
    oss << p1;
    EXPECT_EQ(oss.str(), "(1, 2, 0)");
}

TEST_F(PointTest, IntegerType) {
    EXPECT_EQ(pi.getX(), 1);
    EXPECT_EQ(pi.getY(), 2);
    EXPECT_EQ(pi.getZ(), 0);
}
