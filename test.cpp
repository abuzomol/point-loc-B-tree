#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "LineSegment.h"
#include "MiddleNode.h"
#include "MiddleTree.h"
#include "Node.h"
#include "Point.h"
#include "SuperNode.h"
#include "SuperTree.h"
#include "Tree.h"
#include "gtest/gtest.h"

namespace {

TEST(IntervalTreeTest, IntervalTreeProperlyBuilt)
{
    std::vector<LineSegment> lineSegments;
    lineSegments.reserve(8);
    lineSegments.emplace_back(0, 5, 3, 3);
    lineSegments.emplace_back(1, 4, 6, 6);
    lineSegments.emplace_back(2, 13, 8, 8);
    lineSegments.emplace_back(3, 7, 1, 1);
    lineSegments.emplace_back(6, 9, 7, 7);
    lineSegments.emplace_back(8, 11, 5, 5);
    lineSegments.emplace_back(10, 14, 4, 4);
    lineSegments.emplace_back(12, 15, 2, 2);

    // bool isLess = YLeftLessThan()(lineSegments[0],lineSegments[1]);
    sort(lineSegments.begin(), lineSegments.end(), YLeftLessThan());

    // get all x coordinates from lineSegments
    vector<double> xValues;
    int linesSegSize = lineSegments.size();
    xValues.resize(2 * linesSegSize);
    for (int i = 0; i < linesSegSize; i++)
    {
        xValues[i << 1] = lineSegments[i].getXLeft();
        xValues[(i << 1) + 1] = lineSegments[i].getXRight();
    }
    // sort xValues.
    sort(xValues.begin(), xValues.end());

    // pad with dummy numbers to test boundaries
    //for (int i = 16; i < 25; i++) xValues.push_back(i);

    unsigned int superNodesTotal = ceil(xValues.size() / VAL_SIZE);
    unsigned int superHeight;
    if (superNodesTotal == 1)
        superHeight = 1;
    else
        superHeight = ceil(log(superNodesTotal) / log(CHILD_SIZE)) + 1;

    EXPECT_EQ(2, superHeight );
    SuperTree superTree(superHeight, xValues);
    SuperNode superRoot = superTree.getRoot();
    //test skeleton of the tree
    /*EXPECT_EQ(15, superTree.getSuperTree()[0][0].getVal()[0]);
    EXPECT_EQ(INFTY, superTree.getSuperTree()[0][0].getVal().back());
    EXPECT_EQ(3, superTree.getSuperTree()[1][0].getVal()[0]);
    EXPECT_EQ(7, superTree.getSuperTree()[1][0].getVal()[1]);
    EXPECT_EQ(11, superTree.getSuperTree()[1][0].getVal()[2]);
    EXPECT_EQ(15, superTree.getSuperTree()[1][0].getVal()[3]);
    EXPECT_EQ(23, superTree.getSuperTree()[1][1].getVal()[0]);
    EXPECT_EQ(INFTY, superTree.getSuperTree()[1][1].getVal()[1]);
    */


    EXPECT_EQ(2, ceil(log(16) / log(4)));

    EXPECT_EQ(3, superTree.getSuperTree()[0][0].getVal().front());
    EXPECT_EQ(7, superTree.getSuperTree()[0][0].getVal()[1]);
    EXPECT_EQ(11, superTree.getSuperTree()[0][0].getVal()[2]);
    EXPECT_EQ(15, superTree.getSuperTree()[0][0].getVal()[3]);

    // fill tree
    vector<LineSegment*> lineSegmentss;
    for (auto& lineSeg : lineSegments)
    {
        lineSegmentss.push_back(&lineSeg);
    }
    fillSuperTree(superRoot, lineSegmentss);


    //test left and right and middle trees
    auto leftSemiLines = superRoot.getLeftSemiLines();
    auto leftZeroLines = leftSemiLines[0]->getVal();
    auto leftOneLines = leftSemiLines[1]->getVal();
    auto leftTwoLines = leftSemiLines[2]->getVal();
    auto leftThreeLines = leftSemiLines[3]->getVal();

    EXPECT_EQ(0, leftZeroLines[0].getXLeft());
    EXPECT_EQ(5, leftZeroLines[0].getXRight());
    EXPECT_EQ(3, leftZeroLines[0].getYLeft());
    EXPECT_EQ(1, leftZeroLines[1].getXLeft());
    EXPECT_EQ(4, leftZeroLines[1].getXRight());
    EXPECT_EQ(6, leftZeroLines[1].getYLeft());
    EXPECT_EQ(2, leftZeroLines[2].getXLeft());
    EXPECT_EQ(13, leftZeroLines[2].getXRight());
    EXPECT_EQ(8, leftZeroLines[2].getYLeft());

    EXPECT_EQ(6 , leftOneLines[0].getXLeft());
    EXPECT_EQ(9 , leftOneLines[0].getXRight());
    EXPECT_EQ(7, leftOneLines[0].getYLeft());

    auto middle = superRoot.getMiddle();
    EXPECT_EQ(3,middle->getVal()[0].getXLeft());
    EXPECT_EQ(7,middle->getVal()[0].getXRight());
    EXPECT_EQ(1,middle->getVal()[0].getYLeft());

    EXPECT_EQ(2,middle->getVal()[1].getXLeft());
    EXPECT_EQ(13,middle->getVal()[1].getXRight());
    EXPECT_EQ(8,middle->getVal()[1].getYLeft());

    EXPECT_EQ(3,middle->getSpannedSlabs());


    // Test Queries
    std::vector<Point> points;

    points.reserve(9);
    points.emplace_back(7, 1.5);
    points.emplace_back(13, 2.5);
    points.emplace_back(3, 3.5);
    points.emplace_back(10, 4.5);
    points.emplace_back(8, 5.5);
    points.emplace_back(3, 6.5);
    points.emplace_back(8, 7.5);
    points.emplace_back(7, 8.5);
    points.emplace_back(16, 15);

    for (int i = 0; i < lineSegments.size()-1; i++)
    {
        auto query = pointLocationQuery(superRoot, points[i]);
        EXPECT_EQ(lineSegments[i].getXLeft(), query.getXLeft());
        EXPECT_EQ(lineSegments[i].getXRight(), query.getXRight());
        EXPECT_EQ(lineSegments[i].getYLeft(), query.getYLeft());

        //EXPECT_EQ(lineSegments[i], query);
    }

    auto query = pointLocationQuery(superRoot, points[8]);
    EXPECT_EQ(-1, query.getXLeft());
    EXPECT_EQ(-1, query.getXRight());
    EXPECT_EQ(-1, query.getYLeft());
}

}  // namespace
