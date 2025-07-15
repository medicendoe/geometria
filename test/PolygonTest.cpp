#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include "Point/Point.h"
#include "Poligon/Poligon.h"

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
