//
// Created by muzamil on 1/14/20.
//

#include "LineSegment.h"

#include <cmath>

#include "global_const.h"

double LineSegment::getXLeft() const { return xLeft; }

void LineSegment::setXLeft(double xLeft) { LineSegment::xLeft = xLeft; }

double LineSegment::getXRight() const { return xRight; }

void LineSegment::setXRight(double xRight) { LineSegment::xRight = xRight; }

double LineSegment::getYLeft() const { return yLeft; }

void LineSegment::setYLeft(double yLeft) { LineSegment::yLeft = yLeft; }

double LineSegment::getYRight() const { return yLeft; }

void LineSegment::setYRight(double yRight) { LineSegment::yLeft = yLeft; }

bool LineSegment::isHorizontal() {
    return std::fabs(getYLeft() - getYRight()) < EPS;
}

bool operator==(const LineSegment &lineSegment1, const LineSegment &lineSegment2) {
    return lineSegment1.getXLeft() == lineSegment2.getXLeft() && lineSegment1.getXRight() == lineSegment2.getXRight() &&
           lineSegment1.getYLeft() == lineSegment2.getYLeft() && lineSegment1.getYRight() == lineSegment2.getYRight();
}

std::ostream &operator<<(std::ostream &os, const LineSegment &lineSegment) {
    os << '(' << lineSegment.getXLeft() << ',' << lineSegment.getYLeft() << ')'
       << "--" << '(' << lineSegment.getXRight() << ',' << lineSegment.getYRight()
       << ')';
    return os;
}
