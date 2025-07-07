#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Poligon/Poligon.h"

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

class PolygonTest : public ::testing::Test {
protected:
    std::vector<Point<double>> square = {
        Point<double>(0.0, 0.0),
        Point<double>(1.0, 0.0),
        Point<double>(1.0, 1.0),
        Point<double>(0.0, 1.0)
    };
    
    std::vector<Point<double>> triangle = {
        Point<double>(0.0, 0.0),
        Point<double>(2.0, 0.0),
        Point<double>(1.0, 2.0)
    };
    
    std::vector<Point<double>> clockwise_square = {
        Point<double>(0.0, 0.0),
        Point<double>(0.0, 1.0),
        Point<double>(1.0, 1.0),
        Point<double>(1.0, 0.0)
    };
};

TEST_F(PolygonTest, Constructor) {
    Poligon<double> poly(square);
    EXPECT_EQ(poly.numVertexes(), 4);
}

TEST_F(PolygonTest, NumVertexes) {
    Poligon<double> poly(triangle);
    EXPECT_EQ(poly.numVertexes(), 3);
}

TEST_F(PolygonTest, AreaSquare) {
    Poligon<double> poly(square);
    EXPECT_DOUBLE_EQ(poly.area(), 1.0);
}

TEST_F(PolygonTest, AreaTriangle) {
    Poligon<double> poly(triangle);
    EXPECT_DOUBLE_EQ(poly.area(), 2.0);
}

TEST_F(PolygonTest, IsCCW) {
    Poligon<double> ccw_poly(square);
    Poligon<double> cw_poly(clockwise_square);
    EXPECT_TRUE(ccw_poly.isCCW());
    EXPECT_FALSE(cw_poly.isCCW());
}

TEST_F(PolygonTest, FromCWToCCW) {
    Poligon<double> poly(clockwise_square);
    EXPECT_FALSE(poly.isCCW());
    poly.fromCWToCCW();
    EXPECT_TRUE(poly.isCCW());
}

TEST_F(PolygonTest, IndexOperator) {
    Poligon<double> poly(square);
    Point<double> first = poly[0];
    EXPECT_DOUBLE_EQ(first.getX(), 0.0);
    EXPECT_DOUBLE_EQ(first.getY(), 0.0);
}

TEST_F(PolygonTest, OutputOperator) {
    Poligon<double> poly(triangle);
    std::ostringstream oss;
    oss << poly;
    EXPECT_EQ(oss.str(), "[(0, 0, 0), (2, 0, 0), (1, 2, 0)]");
}

TEST_F(PolygonTest, EmptyPolygon) {
    std::vector<Point<double>> empty;
    Poligon<double> poly(empty);
    EXPECT_EQ(poly.numVertexes(), 0);
    EXPECT_DOUBLE_EQ(poly.area(), 0.0);
}

TEST_F(PolygonTest, IntegerPolygon) {
    std::vector<Point<int>> int_square = {
        Point<int>(0, 0),
        Point<int>(2, 0),
        Point<int>(2, 2),
        Point<int>(0, 2)
    };
    Poligon<int> poly(int_square);
    EXPECT_EQ(poly.area(), 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
