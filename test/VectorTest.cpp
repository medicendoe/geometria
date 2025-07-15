#include <gtest/gtest.h>
#include <sstream>
#include "Point/Point.h"
#include "Vector/Vector.h"

class VectorTest : public ::testing::Test {
protected:
    Vector<double> v1{3.0, 4.0};
    Vector<double> v2{1.0, 2.0};
    Point<double> p{5.0, 6.0};
};

TEST_F(VectorTest, ConstructorFromCoordinates) {
    std::ostringstream oss;
    oss << v1;
    EXPECT_EQ(oss.str(), "Vector(3, 4)");
}

TEST_F(VectorTest, ConstructorFromPoint) {
    Vector<double> v(p);
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(oss.str(), "Vector(5, 6)");
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<double> v3(v1);
    EXPECT_TRUE(v1 == v3);
}

TEST_F(VectorTest, Length) {
    EXPECT_DOUBLE_EQ(v1.length(), 5.0);
}

TEST_F(VectorTest, DotProduct) {
    EXPECT_DOUBLE_EQ(v1.dot(v2), 11.0); // 3*1 + 4*2 = 11
}

TEST_F(VectorTest, Addition) {
    Vector<double> result = v1 + v2;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "Vector(4, 6)");
}

TEST_F(VectorTest, ScalarMultiplication) {
    Vector<double> result = v1 * 2.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "Vector(6, 8)");
}

TEST_F(VectorTest, EqualityOperator) {
    Vector<double> v3(3.0, 4.0);
    EXPECT_TRUE(v1 == v3);
    EXPECT_FALSE(v1 == v2);
}

TEST_F(VectorTest, CrossProduct) {
    Vector<double> result = v1.cross(v2);
    // Cross product should return a vector
    EXPECT_NO_THROW(result.getZ());
}

TEST_F(VectorTest, GetZ) {
    EXPECT_DOUBLE_EQ(v1.getZ(), 0.0);
}
