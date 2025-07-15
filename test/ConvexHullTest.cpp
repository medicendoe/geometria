#include <gtest/gtest.h>
#include <vector>
#include "Point/Point.h"
#include "Poligon/Poligon.h"
#include "ConvexHullStrategy/GiftWrappingAlgorithm/GiftWrappingAlgorithm.h"
#include "ConvexHullStrategy/DivideAndConquerAlgorithm/DivideAndConquerAlgorithm.h"

class ConvexHullTest : public ::testing::Test {
protected:
    // Simple square points
    std::vector<Point<double>> squarePoints = {
        Point<double>(0.0, 0.0),
        Point<double>(2.0, 0.0),
        Point<double>(2.0, 2.0),
        Point<double>(0.0, 2.0)
    };
    
    // Points with some inside the convex hull
    std::vector<Point<double>> pointsWithInterior = {
        Point<double>(0.0, 0.0),
        Point<double>(4.0, 0.0),
        Point<double>(4.0, 4.0),
        Point<double>(0.0, 4.0),
        Point<double>(2.0, 2.0),  // interior point
        Point<double>(1.0, 1.0),  // interior point
        Point<double>(3.0, 1.0),  // interior point
    };
    
    // Triangle points
    std::vector<Point<double>> trianglePoints = {
        Point<double>(0.0, 0.0),
        Point<double>(3.0, 0.0),
        Point<double>(1.5, 3.0)
    };
    
    // Collinear points
    std::vector<Point<double>> collinearPoints = {
        Point<double>(0.0, 0.0),
        Point<double>(1.0, 0.0),
        Point<double>(2.0, 0.0),
        Point<double>(3.0, 0.0)
    };
    
    // Single point
    std::vector<Point<double>> singlePoint = {
        Point<double>(1.0, 1.0)
    };
    
    // Two points
    std::vector<Point<double>> twoPoints = {
        Point<double>(0.0, 0.0),
        Point<double>(2.0, 2.0)
    };
};

TEST_F(ConvexHullTest, GiftWrappingSquare) {
    GiftWrappingAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(squarePoints);
    
    EXPECT_EQ(hull.numVertexes(), 4);
    EXPECT_DOUBLE_EQ(hull.area(), 4.0);
}

TEST_F(ConvexHullTest, GiftWrappingWithInteriorPoints) {
    GiftWrappingAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(pointsWithInterior);
    
    EXPECT_EQ(hull.numVertexes(), 4);  // Should only include corner points
    EXPECT_DOUBLE_EQ(hull.area(), 16.0);
}

TEST_F(ConvexHullTest, GiftWrappingTriangle) {
    GiftWrappingAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(trianglePoints);
    
    EXPECT_EQ(hull.numVertexes(), 3);
    EXPECT_DOUBLE_EQ(hull.area(), 4.5);
}

TEST_F(ConvexHullTest, GiftWrappingSinglePoint) {
    GiftWrappingAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(singlePoint);
    
    EXPECT_EQ(hull.numVertexes(), 1);
    EXPECT_DOUBLE_EQ(hull.area(), 0.0);
}

TEST_F(ConvexHullTest, GiftWrappingTwoPoints) {
    GiftWrappingAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(twoPoints);
    
    EXPECT_EQ(hull.numVertexes(), 2);
    EXPECT_DOUBLE_EQ(hull.area(), 0.0);
}

TEST_F(ConvexHullTest, GiftWrappingCollinear) {
    GiftWrappingAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(collinearPoints);
    
    EXPECT_EQ(hull.numVertexes(), 2);  // All collinear points
    EXPECT_DOUBLE_EQ(hull.area(), 0.0);
}

TEST_F(ConvexHullTest, DivideAndConquerSquare) {
    DivideAndConquerAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(squarePoints);
    
    EXPECT_EQ(hull.numVertexes(), 4);
    EXPECT_DOUBLE_EQ(hull.area(), 4.0);
}

TEST_F(ConvexHullTest, DivideAndConquerWithInteriorPoints) {
    DivideAndConquerAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(pointsWithInterior);

    EXPECT_EQ(hull.numVertexes(), 4);  // Should only include corner points
    EXPECT_DOUBLE_EQ(hull.area(), 16.0);
}

TEST_F(ConvexHullTest, DivideAndConquerTriangle) {
    DivideAndConquerAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(trianglePoints);
    
    EXPECT_EQ(hull.numVertexes(), 3);
    EXPECT_DOUBLE_EQ(hull.area(), 4.5);
}

TEST_F(ConvexHullTest, DivideAndConquerSinglePoint) {
    DivideAndConquerAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(singlePoint);
    
    EXPECT_EQ(hull.numVertexes(), 1);
    EXPECT_DOUBLE_EQ(hull.area(), 0.0);
}

TEST_F(ConvexHullTest, DivideAndConquerTwoPoints) {
    DivideAndConquerAlgorithm<double> algorithm;
    Poligon<double> hull = algorithm.apply(twoPoints);
    
    EXPECT_EQ(hull.numVertexes(), 2);
    EXPECT_DOUBLE_EQ(hull.area(), 0.0);
}

TEST_F(ConvexHullTest, CompareAlgorithms) {
    GiftWrappingAlgorithm<double> giftWrap;
    DivideAndConquerAlgorithm<double> divideConquer;
    
    Poligon<double> hullGift = giftWrap.apply(pointsWithInterior);
    Poligon<double> hullDivide = divideConquer.apply(pointsWithInterior);
    
    // Both algorithms should produce hulls with same area
    EXPECT_DOUBLE_EQ(hullGift.area(), hullDivide.area());
    EXPECT_EQ(hullGift.numVertexes(), hullDivide.numVertexes());
}

TEST_F(ConvexHullTest, IntegerPoints) {
    std::vector<Point<int>> intPoints = {
        Point<int>(0, 0),
        Point<int>(3, 0),
        Point<int>(3, 3),
        Point<int>(0, 3),
        Point<int>(1, 1)  // interior point
    };
    
    GiftWrappingAlgorithm<int> algorithm;
    Poligon<int> hull = algorithm.apply(intPoints);
    
    EXPECT_EQ(hull.numVertexes(), 4);
    EXPECT_EQ(hull.area(), 9);
}
