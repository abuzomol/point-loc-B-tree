//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_LINESEGMENT_H
#define POINT_LOC_B_TREE_LINESEGMENT_H

#include <ostream>

#include "global_const.h"

class LineSegment
{
   private:
    double xLeft, xRight, yLeft;

   public:
    // assume for now all coordinates are positives
    LineSegment() : xLeft(INFTY), xRight(-1), yLeft(INFTY) {}

    LineSegment(double xLeft, double xRight, double yLeft, double yRight)
        : xLeft(xLeft), xRight(xRight), yLeft(yLeft)
    {
    }


    double getXLeft() const;

    void setXLeft(double xLeft);

    double getXRight() const;

    void setXRight(double xRight);

    double getYLeft() const;

    void setYLeft(double yLeft);

    double getYRight() const;

    void setYRight(double yRight);

    void setAll(double xLeft, double xRight, double yLeft);

    bool isHorizontal();

    friend std::ostream& operator<<(std::ostream& os,
                                    const LineSegment& lineSegment);

    friend bool operator==(const LineSegment& lineSegment1,
                           const LineSegment& lineSegment2);
};

bool operator==(const LineSegment& lineSegment1,
                const LineSegment& lineSegment2);

std::ostream& operator<<(std::ostream& os, const LineSegment& lineSegment);

struct XLeftLessThan
{
    bool operator()(const LineSegment& a, const LineSegment& b)
    {
        return a.getXLeft() < b.getXLeft();
    }
};

struct XRightLessThan
{
    bool operator()(const LineSegment& a, const LineSegment& b)
    {
        return a.getXRight() < b.getXRight();
    }
};

struct YLeftLessThan
{
    bool operator()(const LineSegment& a, const LineSegment& b)
    {
        return a.getYLeft() < b.getYLeft();
    }
};

struct YRightLessThan
{
    bool operator()(const LineSegment& a, const LineSegment& b)
    {
        return a.getYRight() < b.getYRight();
    }
};

#endif  // POINT_LOC_B_TREE_LINESEGMENT_H
